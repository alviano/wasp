input = """
sat|e37|e34.
e34:-sat.
e37:-sat.
sat|e32.
sat:-e34.
sat|e25.
e33:-sat.
sat:-e33.
e50:-sat.
sat:-e50.
e20:-sat.
sat:-e20.
sat:-e20.
sat:-e20.
e12:-sat.
sat|e20:-e12.
e59:-sat.
sat:-e59.
e26:-sat.
sat:-e26.
e55:-sat.
sat:-e55.
e21:-sat.
sat|e55:-e21.
e36:-sat.
sat:-e36.
e25:-sat.
e32:-sat.
e38:-sat.
e31:-sat.
e24:-sat.
e30:-sat.
:-not sat.
"""
output = """
"""
