import json
import pytest

from subprocess import run, PIPE

rules = json.loads(open('test/unit_tests/parser_data.json', 'rb').read())

parameters = []

for x in rules:
    output = run(['./ifj18'], stdout=PIPE, input=x['input'], encoding='utf-8')

    parameters.append(pytest.param(output.stdout, x))

@pytest.mark.parametrize('test_input,expected', parameters)
def test_tokens(test_input, expected):
    assert test_input == expected["output"], f"test failed: {expected['name']}"
