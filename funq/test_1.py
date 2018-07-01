#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
This file is part of the funq tutorial. It briefly shows widget
interaction.
"""

from funq.testcase import FunqTestCase
from funq.client import FunqError


class TestCase2(FunqTestCase):
    # identify the configuration
    __app_config_name__ = 'librepcb'

    def __init__(self, *args, **kwargs):
        FunqTestCase.__init__(self, *args, **kwargs)
        self.__app_config__.args = ['/media/Daten/Eigene_Dateien/Programmieren/Qt/LibrePCB/LibrePCB/tests/data/project/boards/BoardPlaneFragmentsBuilderTest/test_project/test_project.lpp']

    #def test_my_first_test(self):
        # this will write a "dump.json" file
    #    self.funq.dump_widgets_list('dump.json')


    # def test_open_dialog(self):
    #     """
    #     Test that a dialog is open when user click on the test button.
    #     """
    #     self.funq.widget('btnNewProject').click()
    #     #import time
    #     #time.sleep(3)
    #     #self.funq.dump_widgets_list('dump.json')
    #
    #     dialog = self.funq.widget('newProjectWizardWindow')
    #
    #     self.assertEquals(dialog.properties()['visible'], True)

    def test_dclick(self):
        import time
        time.sleep(3)
        graphicsView = self.funq.widget('schematicGraphicsView')
        graphicsView.dump_gitems()
        for item in graphicsView.gitems().iter():
            print(item.objectname)
            item.dclick()
            time.sleep(0.4)
