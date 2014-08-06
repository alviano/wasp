input = """
% This originally tested the old checker (command-line -OMb-) which does not
% exist any longer.
strategic(4) | strategic(16).
strategic(3) | strategic(15).
strategic(15) | strategic(6).
strategic(15) | strategic(2).
strategic(12) | strategic(16).
strategic(8) | strategic(2).
strategic(8) | strategic(15).
strategic(3) | strategic(8).
strategic(5) | strategic(2).
strategic(4) | strategic(17).
strategic(4) | strategic(13).
strategic(5) | strategic(10).
strategic(1) | strategic(2).
strategic(10) | strategic(13).
strategic(4) | strategic(13).
strategic(15) | strategic(5).
strategic(9) | strategic(17).
strategic(2) | strategic(3).
strategic(2) | strategic(16).
strategic(2) | strategic(13).
strategic(13) | strategic(10).
strategic(14) | strategic(1).
strategic(4) | strategic(17).
strategic(8) | strategic(9).
strategic(16) | strategic(12).
strategic(5) | strategic(1).
strategic(6) | strategic(8).
strategic(10) | strategic(4).
strategic(7) | strategic(1).
strategic(15) | strategic(10).
strategic(12) | strategic(11).
strategic(15) | strategic(2).
strategic(7) | strategic(14).
strategic(2) | strategic(8).
strategic(11) | strategic(4).
strategic(1) | strategic(4).
strategic(13) | strategic(14).
strategic(2) | strategic(16).
strategic(10) | strategic(9).
strategic(4) | strategic(7).
strategic(2) | strategic(7).
strategic(7) | strategic(5).
strategic(12) | strategic(14).
strategic(7) | strategic(18).
strategic(15) | strategic(3).
strategic(10) | strategic(6).
strategic(11) | strategic(4).
strategic(5) | strategic(17).
strategic(15) | strategic(6).
strategic(4) | strategic(7).
strategic(7) | strategic(4).
strategic(8) | strategic(1).
strategic(16) | strategic(9).
strategic(15) | strategic(7).
strategic(14) :- strategic(1), strategic(9), strategic(11).
strategic(14) :- strategic(1), strategic(13), strategic(8).
strategic(13) :- strategic(3), strategic(4), strategic(7).
strategic(13) :- strategic(3), strategic(14), strategic(18).
strategic(15) :- strategic(3), strategic(11), strategic(17).
strategic(1) :- strategic(4), strategic(2), strategic(2).
strategic(3) :- strategic(4), strategic(7), strategic(13).
strategic(3) :- strategic(4), strategic(17), strategic(11).
strategic(18) :- strategic(4), strategic(2), strategic(2).
strategic(8) :- strategic(5), strategic(13), strategic(12).
strategic(8) :- strategic(5), strategic(13), strategic(17).
strategic(12) :- strategic(5), strategic(3), strategic(3).
strategic(16) :- strategic(5), strategic(9), strategic(9).
strategic(4) :- strategic(6), strategic(6), strategic(6).
strategic(17) :- strategic(7), strategic(18), strategic(3).
strategic(11) :- strategic(12), strategic(15), strategic(15).
strategic(5) :- strategic(13), strategic(10), strategic(12).
strategic(7) :- strategic(14), strategic(5), strategic(4).
strategic(7) :- strategic(14), strategic(8), strategic(1).
strategic(2) :- strategic(15), strategic(3), strategic(10).
strategic(2) :- strategic(15), strategic(3), strategic(12).
strategic(10) :- strategic(15), strategic(5), strategic(5).
strategic(6) :- strategic(16), strategic(13), strategic(13).
strategic(9) :- strategic(16), strategic(5), strategic(5).
strategic(9) :- strategic(16), strategic(15), strategic(15).
"""
output = """
{strategic(4), strategic(16), strategic(3), strategic(6), strategic(2), strategic(12), strategic(8), strategic(5), strategic(17), strategic(13), strategic(10), strategic(1), strategic(9), strategic(14), strategic(7), strategic(18)}
{strategic(4), strategic(16), strategic(15), strategic(2), strategic(8), strategic(5), strategic(10), strategic(1), strategic(9), strategic(14), strategic(7), strategic(11), strategic(18)}
{strategic(4), strategic(16), strategic(3), strategic(15), strategic(6), strategic(2), strategic(17), strategic(13), strategic(10), strategic(1), strategic(9), strategic(14), strategic(7), strategic(11), strategic(18)}
"""
