input = """
% Strings starting with "aux" may occur in regular input, even if (in some)
% versions of DLV/proposed patches they can occur as a result of internal
% predicate names as well.

aux v aux1(1) v auxiliary(1,2).
"""

output = """
{aux1(1)}
{auxiliary(1,2)}
{aux}
"""
