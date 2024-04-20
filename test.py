import os
import subprocess

test_directory = 'testcases/phase2'
output_directory = 'output/'
for filename in os.listdir(test_directory):
    if not filename.startswith('in'): continue
    test_number = filename[2:filename.find('.')]
    input_filepath = os.path.join(test_directory, filename)
    output_filepath = os.path.join(output_directory, 'out{}.txt'.format(test_number))
    verify_filepath = input_filepath.replace('in', 'out').replace('.py', '.txt')
    with open(output_filepath, 'w') as file:
        subprocess.run(['./mypython.exe', input_filepath], stdout=file)
        file.close()
    with open(output_filepath, 'r') as file1, open(verify_filepath, 'r') as file2:
        status = 'passed' if file1.readlines() == file2.readlines() else 'failed'
        print('Test {} {}.'.format(test_number, status))
        file1.close()
        file2.close()