input = """
a v b :- not c.
b :- d, not c.
e v a :- f.
g v d :- h.
i v g :- j.
e v a v k :- l.
l v e v a :- l, i.
j v not_i :- g.
c v m :- i.
h :- k.
n v i v e.
o v h.
o v p.
q v o v f.
q v n v c.
not_h v not_c.
g v r.

not_e :- not e.
not_a :- not a.
not_q :- not q.
not_b :- not b.
:- i, not_i.
:- c, not_c.

"""

output = """
{a, b, d, f, h, n, not_c, not_e, not_q, p, r}
{a, b, d, f, h, n, not_e, not_h, not_q, p, r}
{a, c, d, f, h, i, not_b, not_e, not_h, not_q, p, r}
{a, c, f, g, h, i, j, not_b, not_e, not_h, not_q, p}
{a, e, g, h, j, not_b, not_c, p, q}
{a, e, g, h, j, not_b, not_h, p, q}
{a, e, g, h, not_b, not_c, not_i, p, q}
{a, e, g, h, not_b, not_h, not_i, p, q}
{a, e, g, j, not_b, not_c, o, q}
{a, e, g, j, not_b, not_h, o, q}
{a, e, g, not_b, not_c, not_i, o, q}
{a, e, g, not_b, not_h, not_i, o, q}
{a, e, not_b, not_c, o, q, r}
{a, e, not_b, not_h, o, q, r}
{a, f, g, h, j, n, not_b, not_c, not_e, not_q, p}
{a, f, g, h, j, n, not_b, not_e, not_h, not_q, p}
{a, f, g, h, n, not_b, not_c, not_e, not_i, not_q, p}
{a, f, g, h, n, not_b, not_e, not_h, not_i, not_q, p}
{a, g, h, i, j, m, not_b, not_c, not_e, p, q}
{a, g, h, i, j, m, not_b, not_e, not_h, p, q}
{a, g, h, j, n, not_b, not_c, not_e, p, q}
{a, g, h, j, n, not_b, not_e, not_h, p, q}
{a, g, h, n, not_b, not_c, not_e, not_i, p, q}
{a, g, h, n, not_b, not_e, not_h, not_i, p, q}
{a, g, i, j, m, not_b, not_c, not_e, o, q}
{a, g, i, j, m, not_b, not_e, not_h, o, q}
{a, g, j, n, not_b, not_c, not_e, not_q, o}
{a, g, j, n, not_b, not_e, not_h, not_q, o}
{a, g, n, not_b, not_c, not_e, not_i, not_q, o}
{a, g, n, not_b, not_e, not_h, not_i, not_q, o}
{a, i, m, not_b, not_c, not_e, o, q, r}
{a, i, m, not_b, not_e, not_h, o, q, r}
{a, n, not_b, not_c, not_e, not_q, o, r}
{a, n, not_b, not_e, not_h, not_q, o, r}
{b, d, e, f, h, n, not_a, not_c, not_q, p, r}
{b, d, e, f, h, n, not_a, not_h, not_q, p, r}
{b, d, e, h, not_a, not_c, p, q, r}
{b, d, e, h, not_a, not_h, p, q, r}
{b, d, h, i, m, not_a, not_c, not_e, p, q, r}
{b, d, h, i, m, not_a, not_e, not_h, p, q, r}
{b, d, h, n, not_a, not_c, not_e, p, q, r}
{b, d, h, n, not_a, not_e, not_h, p, q, r}
{b, e, f, g, h, j, n, not_a, not_c, not_q, p}
{b, e, f, g, h, j, n, not_a, not_h, not_q, p}
{b, e, f, g, h, n, not_a, not_c, not_i, not_q, p}
{b, e, f, g, h, n, not_a, not_h, not_i, not_q, p}
{b, e, g, h, j, not_a, not_c, p, q}
{b, e, g, h, j, not_a, not_h, p, q}
{b, e, g, h, not_a, not_c, not_i, p, q}
{b, e, g, h, not_a, not_h, not_i, p, q}
{b, e, g, j, not_a, not_c, o, q}
{b, e, g, j, not_a, not_h, o, q}
{b, e, g, not_a, not_c, not_i, o, q}
{b, e, g, not_a, not_h, not_i, o, q}
{b, e, not_a, not_c, o, q, r}
{b, e, not_a, not_h, o, q, r}
{b, g, h, i, j, m, not_a, not_c, not_e, p, q}
{b, g, h, i, j, m, not_a, not_e, not_h, p, q}
{b, g, h, j, n, not_a, not_c, not_e, p, q}
{b, g, h, j, n, not_a, not_e, not_h, p, q}
{b, g, h, n, not_a, not_c, not_e, not_i, p, q}
{b, g, h, n, not_a, not_e, not_h, not_i, p, q}
{b, g, i, j, m, not_a, not_c, not_e, o, q}
{b, g, i, j, m, not_a, not_e, not_h, o, q}
{b, g, j, n, not_a, not_c, not_e, not_q, o}
{b, g, j, n, not_a, not_e, not_h, not_q, o}
{b, g, n, not_a, not_c, not_e, not_i, not_q, o}
{b, g, n, not_a, not_e, not_h, not_i, not_q, o}
{b, i, m, not_a, not_c, not_e, o, q, r}
{b, i, m, not_a, not_e, not_h, o, q, r}
{b, n, not_a, not_c, not_e, not_q, o, r}
{b, n, not_a, not_e, not_h, not_q, o, r}
{c, d, e, f, h, not_a, not_b, not_h, not_q, p, r}
{c, d, h, i, not_a, not_b, not_e, not_h, p, q, r}
{c, e, f, g, h, j, not_a, not_b, not_h, not_q, p}
{c, e, f, g, h, not_a, not_b, not_h, not_i, not_q, p}
{c, e, g, j, not_a, not_b, not_h, not_q, o}
{c, e, g, not_a, not_b, not_h, not_i, not_q, o}
{c, e, not_a, not_b, not_h, not_q, o, r}
{c, g, h, i, j, not_a, not_b, not_e, not_h, p, q}
{c, g, i, j, not_a, not_b, not_e, not_h, not_q, o}
{c, i, not_a, not_b, not_e, not_h, not_q, o, r}
"""
