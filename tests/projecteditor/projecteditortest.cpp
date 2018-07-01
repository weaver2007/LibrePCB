/*
 * LibrePCB - Professional EDA for everyone!
 * Copyright (C) 2013 LibrePCB Developers, see AUTHORS.md for contributors.
 * http://librepcb.org/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*****************************************************************************************
 *  Includes
 ****************************************************************************************/
#include <QtCore>
#include <QtConcurrent>
#include <QtTest/QtTest>
#include <gtest/gtest.h>
#include <librepcb/common/graphics/graphicsview.h>
#include <librepcb/project/project.h>
#include <librepcb/project/schematics/schematic.h>
#include <librepcb/project/schematics/items/si_symbol.h>
#include <librepcb/projecteditor/projecteditor.h>
#include <librepcb/projecteditor/schematiceditor/schematiceditor.h>
#include <librepcb/workspace/workspace.h>

/*****************************************************************************************
 *  Namespace
 ****************************************************************************************/
namespace librepcb {
namespace project {
namespace editor {
namespace tests {

/*****************************************************************************************
 *  Test Class
 ****************************************************************************************/

class ProjectEditorTest : public ::testing::Test
{
    protected:
        GraphicsView* getGraphicsView(SchematicEditor* se) {
            return se->mGraphicsView;
        }
};

/*****************************************************************************************
 *  Test Methods
 ****************************************************************************************/

bool asyncCloseModalDialog() {
    QElapsedTimer t;
    t.start();
    while (t.elapsed() < 10000) { // too small timeout leads to never ending test!
        if (QDialog* dialog = dynamic_cast<QDialog*>(qApp->activeModalWidget())) {
            // modal dialog found, show it a short time and then post event to close it
            QThread::msleep(500);
            QApplication::postEvent(dialog, new QEvent(QEvent::Close));
            return true;
        }
    }
    return false;
}

TEST_F(ProjectEditorTest, testGraphicsViewDoubleClick)
{
    FilePath testDataDir(TEST_DATA_DIR "/project/boards/BoardPlaneFragmentsBuilderTest");

    // open project from test data directory
    FilePath projectFp = testDataDir.getPathTo("test_project/test_project.lpp");
    Project project(projectFp, true);

    // open workspace
    FilePath workspaceFp = FilePath::getRandomTempPath();
    workspace::Workspace::createNewWorkspace(workspaceFp);
    workspace::Workspace workspace(workspaceFp);

    // open project editor
    ProjectEditor editor(workspace, project);
    editor.showAllRequiredEditors();

    // open schematic editor
    SchematicEditor* schEditor = editor.getSchematicEditor();
    GraphicsView* schView = getGraphicsView(schEditor);
    QTest::qWait(1000); // wait until gui is stable

    // get a symbol and center it
    SI_Symbol* symbol = project.getSchematicByIndex(0)->getSymbols().first();
    schView->centerOn(symbol->getPosition().toPxQPointF());

    // simulate double click and check if symbol properties dialog has appeared
    QFuture<bool> dialogAppeared = QtConcurrent::run(&asyncCloseModalDialog);
    QTest::mousePress(schView->viewport(), Qt::LeftButton);
    QTest::mouseRelease(schView->viewport(), Qt::LeftButton);
    QTest::mouseDClick(schView->viewport(), Qt::LeftButton);
    EXPECT_TRUE(dialogAppeared.result());
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace tests
} // namespace editor
} // namespace project
} // namespace librepcb
