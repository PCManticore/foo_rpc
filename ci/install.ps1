# Sample script to install Python and pip under Windows
# Authors: Olivier Grisel, Jonathan Helmus, Kyle Kastner, and Alex Willmer
# License: CC0 1.0 Universal: http://creativecommons.org/publicdomain/zero/1.0/

$GET_PIP_URL = "https://bootstrap.pypa.io/get-pip.py"
$GET_PIP_PATH = "C:\get-pip.py"
$LATEST_AND_GREATEST = "https://www.python.org/ftp/python/3.6.0/python-3.6.0b1-amd64.exe"


function Download ($filename, $url) {
    $webclient = New-Object System.Net.WebClient

    $basedir = $pwd.Path + "\"
    $filepath = $basedir + $filename
    if (Test-Path $filename) {
        Write-Host "Reusing" $filepath
        return $filepath
    }

    # Download and retry up to 3 times in case of network transient errors.
    Write-Host "Downloading" $filename "from" $url
    $retry_attempts = 2
    for ($i = 0; $i -lt $retry_attempts; $i++) {
        try {
            $webclient.DownloadFile($url, $filepath)
            break
        }
        Catch [Exception]{
            Start-Sleep 1
        }
    }
    if (Test-Path $filepath) {
        Write-Host "File saved at" $filepath
    } else {
        # Retry once to get the error message if any at the last try
        $webclient.DownloadFile($url, $filepath)
    }
    return $filepath
}



function DownloadPython ($url) {
    $filepath = Download "python.exe" $url
    return $filepath
}


function InstallPython ($url) {

    $installer_path = DownloadPython $url
    InstallPythonEXE $installer_path
}


function InstallPythonEXE ($exepath) {
    $install_args = "/quiet /passive"
    RunCommand $exepath $install_args
}


function RunCommand ($command, $command_args) {
    Write-Host $command $command_args
    Start-Process -FilePath $command -ArgumentList $command_args -Wait -Passthru
}

function main () {
    InstallPython $LATEST_AND_GREATEST
    Start-Process -FilePath "python" -ArgumentList "-m ensurepip"
}

main
