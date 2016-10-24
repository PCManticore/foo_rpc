from pywinauto.application import Application
from pywinauto import findwindows

app = Application().start("C:\\projects\\foo-rpc\\portable_test\\foobar2000.exe /config")
window = app.top_window_()['Tree2']
window.GetItem(['Playback']).Click()  #Click on a top level element
window.GetItem(['Playback', 'Output']).Click()  #Click on a sub element
output_page = app.top_window_()
output_page['ComboBox'].Select('Null Output')
output_page['OK'].Click()
