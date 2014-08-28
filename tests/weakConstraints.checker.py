def decodeModels(string):
    models = []        

    for model in string.split("\n"):
        model = model.strip()
        if len(model) > 5 and model[:4] == "COST":
            models.append(model.split("COST "))

    if( len(models) > 0 ):
        return models[-1]
    else:
        return ""
    
def checker(actualOutput, actualError):
    global output
    expectedModels = decodeModels(output)

    if actualError:
        reportFailure(expectedModels, actualError)
        return

    if not actualOutput:
        reportFailure(expectedModels, "No output stream!")
        return
    
    actualModels = decodeModels(actualOutput)
    #print(expectedModels)
    #print(actualModels)
    if expectedModels != actualModels:
        reportFailure(expectedModels, actualModels)
    else:
        reportSuccess(expectedModels, actualModels)
