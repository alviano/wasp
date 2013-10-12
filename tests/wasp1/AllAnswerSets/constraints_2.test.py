input = """
% The constraint here can be detected as "always violated" already
% before the grounding.

a.

:- not c.

"""

output = """
"""
