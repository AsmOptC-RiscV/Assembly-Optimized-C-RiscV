import serial
import subprocess

"""
Generic bench setup
"""
class Platform:

    def __init__(self, debug=False):
        self.debug = debug

    def clean(self):
        if self.debug:
            subprocess.call(["make", "clean"])
        else:
            subprocess.run(["make", "clean"], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

    def compile(self):
        if self.debug:
            subprocess.call(["make"])
        else:
            subprocess.run(["make"], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

    def build(self, scheme, compiler):
        raise NotImplementedError()

    def exec(self):
        raise NotImplementedError()

    def name(self):
        raise NotImplementedError()

"""
riscOVPsim bench setup
"""
class riscvOVPSim(Platform):
    def name(self):
        return 'riscvOVPSim'

    def build(self, scheme, compiler):
        if self.debug:
            subprocess.call(["./configure.py", scheme, compiler, "riscvOVPsim"], stdin=None, stdout=None, stderr=None, shell=False)
        else:
            subprocess.run(["./configure.py", scheme, compiler, "riscvOVPsim"], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

    def exec(self):
        p2 = subprocess.run(["make", "startsim"], capture_output=True, encoding="utf-8")
        output = p2.stdout.split("\n")
        output = output[19:-26]
        return output

"""
SiFive bench setup
"""
class SiFive(Platform):
    def name(self):
        return 'SiFive'

    def build(self, scheme, compiler):
        if self.debug:
            subprocess.call(["./configure.py", scheme, compiler, "sifive"], stdin=None, stdout=None, stderr=None, shell=False)
        else:
            subprocess.run(["./configure.py", scheme, compiler, "sifive"], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

    def exec(self):
        if self.debug:
            subprocess.call(["make", "upload"])
        else:
            subprocess.run(["make", "upload"], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        output = []
        with serial.Serial('/dev/ttyUSB1', 115200, timeout=1) as ser:
            line_d = ""
            while line_d != "DONE!":
                line = ser.readline()   # read a '\n' terminated line
                line_d = line.decode('utf-8')[1:-1]
                output.append(line_d)
                if self.debug:
                    print(line_d)
        return output
