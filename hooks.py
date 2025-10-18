import json
import subprocess
from typing import Optional

Import("env")

#
# Upload actions
#

ENV_TO_SERIAL = {
    'lock': '2EF3076850305247522E3120FF112711',
    'remote': '',
}

def get_port_for_env(env_name: str) -> Optional[str]:
    # list devices
    proc = subprocess.Popen(['pio', 'device', 'list', '--serial', '--json-output'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = proc.communicate()
    if proc.returncode != 0:
        raise Exception(f"Error listing devices: {stderr.decode()}")
    
    devices = json.loads(stdout)

    # print HWIDs
    print("Available devices:")
    for dev in devices:
        print(f"  HWID: {dev['hwid']}, Port: {dev['port']}")
    
    # find entry for env
    serial = ENV_TO_SERIAL[env_name]
    entry = None
    for dev in devices:
        if serial in dev['hwid']:
            entry = dev
            break
    if entry is None:
        return None

    # return port
    return entry['port']

def before_upload(source, target, env):
    print("before_upload")

    env_name = env['PIOENV']
    if env_name not in ENV_TO_SERIAL:
        return
    
    port = get_port_for_env(env_name)
    if port is None:
        print(f"ERROR: No device found for environment '{env_name}'")
        Exit(1)

    env.Replace(UPLOAD_PORT=port)
    env.Replace(MONITOR_PORT=port)

env.AddPreAction("upload", before_upload)