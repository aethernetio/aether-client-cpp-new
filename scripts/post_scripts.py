import os
import platform
import subprocess

def run_library_script(*args, **kwargs):
    print("run_library_script!")
    # Get info about OS
    os_info = platform.system()
    path = os.path.dirname(os.path.realpath(__file__))
    # Paths components
    components = path.split(os.sep)
    script_cwd = os.sep.join(components[:-1])
    if os_info == 'Windows':
        print("Script runs on Windows")
        script_path = os.path.join(script_cwd, "git_init.ps1")
        subprocess.run(["powershell.exe", "-NoProfile", "-File", script_path], cwd=script_cwd, check=True)
    elif os_info == 'Linux':
        print("Script runs on Linux")
        script_path = os.path.join(script_cwd, "git_init.sh")
        subprocess.run(['sh', script_path])
    elif os_info == 'Darwin':
        print("Script runs on macOS")
        script_path = os.path.join(script_cwd, "git_init.sh")
        subprocess.run(['sh', script_path])
    else:
        print(f"Unknown OS: {os_info}")


if __name__ == "__main__":
    run_library_script()