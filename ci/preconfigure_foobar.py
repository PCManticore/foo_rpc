from pywinauto.application import Application
from pywinauto import timings

app = Application().start("C:\\projects\\foo-rpc\\portable_test\\foobar2000.exe /config")
quick_config = timings.WaitUntilPasses(
    3, 0.5, lambda: app.window_(title='Quick Appearance Setup'))
quick_config.Close()

window = app.top_window_()['Tree2']
playback = window.GetItem('\\Playback')
playback.Expand()
output = playback.GetChild('Output')
output.Select()
output_page = app.top_window_()
output_page['ComboBox'].Select('Null Output')
output_page['OK'].Click()