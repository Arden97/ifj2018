import json
import pytest

from subprocess import run, PIPE, call

rules = json.loads(open('test/supertests/parser_data.json', 'rb').read())
base_path = rules["basepath_in"]
base_path_out = rules["basepath_out"]

parameters = []

for x in rules["items"]:
    if x['type'] == 'string':
        inp = x['input']
    elif x['type'] == 'file':
        filepath = "{basepath}/{filename}".format(basepath=base_path, filename=x['input'])
    else:
        raise Exception('Unknown type')

    output = run(['./ifj18'], stdin=open(filepath, 'rb'), encoding='utf-8', stdout=PIPE, stderr=PIPE)

    print(output.returncode)

    parameters.append(pytest.param(output, x))


@pytest.mark.parametrize('test_input,expected', parameters)
def test_tokens(test_input, expected):
    if expected['expected'] == 'error':
        assert test_input.returncode == expected["output"]
    elif expected['expected'] == 'success':
        filepath = "{basepath}/{filename}".format(basepath=base_path_out, filename=x['output'])
        das = open('tmp.txt', 'w')
        das.write(test_input.stdout)
        output = run(['./ic18int', '--file'], encoding='utf-8', stdout=PIPE, stderr=PIPE)
        assert output.stdout == open(filepath, 'rb').read()
