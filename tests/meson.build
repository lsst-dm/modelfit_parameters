test_names = [
    'parameter',
]
foreach test_name : test_names
    test = executable(
        'parameters_test_' + test_name,
        'test_' + test_name + '.cc',
        include_directories : public_headers,
    )
    test(test_name, test)
endforeach
