from pywinauto.application import Application

app = Application().start("C:\\projects\\foo-rpc\\portable_test\\foobar2000.exe /config")
window = app.top_window_()['Tree2']
playback = window.GetItem('\\Playback')
playback.Expand()
output = playback.GetChild('Output')
output.Select()
output_page = app.top_window_()
output_page['ComboBox'].Select('Null Output')
output_page['OK'].Click()
