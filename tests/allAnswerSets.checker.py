def decodeModels(string):
    models = []

    for model in string.split("\n"):
        model = model.strip()
        if len(model) > 0 and model[0] == '{' and model[-1] == '}':
            models.append(sorted(model[1:-1].split(", ")))
            
    return sorted(models)
    
def checker(expectedOutput, actualOutput, actualError):
    expectedModels = decodeModels(expectedOutput)
    actualModels = decodeModels(actualOutput)
    if expectedModels != actualModels:
        reportFailure(expectedModels, actualModels)
    else:
        reportSuccess(expectedModels, actualModels)
