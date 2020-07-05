#!/usr/bin/python3

import os
from utils.BenchSetup import riscvOVPSim, SiFive
from utils.tools import read, save, bcolors
from progress.bar import Bar

def parse(output):
    for l in output:
        line = l.split("\t")
        if len(line) > 3:
            if (line[0] == '128' or line[0] == '28') and line[1] == '128':
                return line[2:-1]

def get_fun(scheme):
    functions = {
        'crypto_aead_encrypt' : '',
        'crypto_aead_encrypt_2' : '',
        'crypto_aead_encrypt_3' : '',
        'crypto_aead_encrypt_4' : '',
        'crypto_aead_encrypt_5' : '',
        'crypto_aead_encrypt_6' : '',
        'crypto_aead_encrypt_7' : '',
    }
    with open(scheme + "/base.h") as f:
        content = f.readlines()
    for line in content:
        line = line.strip()
        line = line.split(' ')
        if len(line) == 3:
            if line[0][:2] != '//':
                if line[1][:19] == 'crypto_aead_encrypt':
                    functions[line[1]] = line[2]
    return(functions)

def print_latex_header(benches, compiler):
    output_str = ""
    output_str += "\\begin{center}" + "\n"
    output_str += "\t\\begin{longtable}{l r r r}" + "\n"
    output_str += "\t\\caption{Cycle counts for different ciphers implementations " + compiler + " compiled with \\texttt{-O3} for encrypting 128 bytes of message and 128 bytes of associated data.}" + "\n"
    output_str += "\t\t\t\\\\" + "\n"
    output_str += "\t\t\t\\toprule" + "\n"
    output_str += "\t\t\tImplementation\t\t\t& " + benches[0].name() + "\t& " + benches[1].name() + " \t& Relative\\\\" + "\n"
    output_str += "\t\t\t\\midrule" + "\n"
    return output_str

def print_latex_footer():
    output_str = ""
    output_str += "\t\t\t\\bottomrule" + "\n"
    output_str += "\t\\label{tab:rweather-bench}" + "\n"
    output_str += "\t\\end{longtable}" + "\n"
    output_str += "\\end{center}" + "\n"
    return output_str

def print_latex(datas, ciphers, j):
    output_str = ""
    template = "\t\t\t\\Cipher{{{cipher}}} \t& {riscv} \t& {sifive} \t& {sign}{relative}\\% \\\\\n"
    keys = ['crypto_aead_encrypt',
        'crypto_aead_encrypt_2',
        'crypto_aead_encrypt_3',
        'crypto_aead_encrypt_4',
        'crypto_aead_encrypt_5',
        'crypto_aead_encrypt_6',
        'crypto_aead_encrypt_7']
    i = 0
    for k in keys:
        if ciphers.get(k) != '':
            relative = round((int(datas[1][j][i]) - int(datas[0][j][i]))*100/float(datas[0][j][i]))
            if relative > 0:
                sign = '+'
            else:
                sign = '-'
            output_str += template.format(cipher=ciphers.get(k), riscv=datas[0][j][i], sifive=datas[1][j][i], sign=sign, relative=relative)
            i += 1
    return output_str

# Setup
ciphers = ['rweather/' + f for f in os.listdir('rweather/')]
ciphers.sort()
results = [[],[]]

compiler = "clang-10"
benches = [riscvOVPSim(), SiFive()]

bar = Bar('Processing', max=len(benches) * len(ciphers))

# Measure
for cipher in ciphers:
    b = 0
    for bench in benches:
        bench.clean()
        bench.build(cipher, compiler)
        bench.compile()
        output = bench.exec()
        results[b].append(parse(output))
        b += 1
        bar.next()
        
print("")
print("")
print("")
print("")
print("")
print("")
print("")
print("")
print("")

output_str = ""

# Print
output_str += print_latex_header(benches, compiler)

for j in range(len(ciphers)):
    cipher_names = get_fun(ciphers[j])
    output_str += print_latex(results, cipher_names, j)

output_str += print_latex_footer()

print(output_str)
save('paper/_bench.tex', output_str.replace("_", "\_"))