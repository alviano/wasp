def decodeModels(string):
    models = []

    for model in string.split("\n"):
        model = model.strip()
        if len(model) > 0 and model[0] == '{' and model[-1] == '}':
            models.append(sorted(model[1:-1].split(", ")))
            
    return sorted(models)
    
def checker(actualOutput, actualError):
    global output
    expectedModels = decodeModels(output)
    actualModels = decodeModels(actualOutput)
    #print(expectedModels)
    #print(actualModels)
    if expectedModels != actualModels:
        reportFailure(expectedModels, actualModels)
    else:
        reportSuccess(expectedModels, actualModels)
