input = """
1 2 0 0
1 3 0 0
1 4 0 0
1 5 0 0
1 6 0 0
1 7 2 1 8 9
1 8 2 1 7 9
1 9 0 0
1 10 2 1 11 12
1 11 2 1 10 12
1 12 0 0
1 13 2 1 14 15
1 14 2 1 13 15
1 15 0 0
1 16 2 1 17 18
1 17 2 1 16 18
1 18 0 0
5 20 6 4 0 16 13 10 7 1 2 3 4
5 21 7 4 0 16 13 10 7 1 2 3 4
1 19 2 1 21 20
5 23 5 4 0 16 13 10 7 1 2 3 4
5 24 6 4 0 16 13 10 7 1 2 3 4
1 22 2 1 24 23
5 25 7 4 0 16 13 10 7 1 2 3 4
5 26 8 4 0 16 13 10 7 1 2 3 4
1 22 2 1 26 25
5 28 9 4 0 16 13 10 7 1 2 3 4
1 27 1 1 28
5 30 8 4 0 16 13 10 7 1 2 3 4
1 29 1 1 30
5 32 7 4 0 16 13 10 7 1 2 3 4
1 31 1 1 32
5 34 6 4 0 16 13 10 7 1 2 3 4
1 33 1 1 34
5 36 6 4 0 16 13 10 7 1 2 3 4
1 35 1 0 36
5 38 7 4 0 16 13 10 7 1 2 3 4
1 37 1 0 38
5 40 6 4 0 16 13 10 7 1 2 3 4
1 39 1 0 40
5 42 7 4 0 16 13 10 7 1 2 3 4
1 41 1 0 42
5 44 6 4 0 16 13 10 7 1 2 3 4
5 45 7 4 0 16 13 10 7 1 2 3 4
1 43 2 1 45 44
5 47 5 4 0 16 13 10 7 1 2 3 4
5 48 6 4 0 16 13 10 7 1 2 3 4
1 46 2 1 48 47
5 49 7 4 0 16 13 10 7 1 2 3 4
5 50 8 4 0 16 13 10 7 1 2 3 4
1 46 2 1 50 49
5 52 9 4 0 16 13 10 7 1 2 3 4
1 51 1 1 52
5 54 8 4 0 16 13 10 7 1 2 3 4
1 53 1 1 54
5 56 7 4 0 16 13 10 7 1 2 3 4
1 55 1 1 56
5 58 6 4 0 16 13 10 7 1 2 3 4
1 57 1 1 58
5 60 6 4 0 16 13 10 7 1 2 3 4
1 59 1 0 60
5 62 7 4 0 16 13 10 7 1 2 3 4
1 61 1 0 62
5 64 6 4 0 16 13 10 7 1 2 3 4
1 63 1 0 64
5 66 7 4 0 16 13 10 7 1 2 3 4
1 65 1 0 66
5 68 6 4 0 16 13 10 7 1 2 3 4
5 69 7 4 0 16 13 10 7 1 2 3 4
1 67 2 1 69 68
5 71 5 4 0 16 13 10 7 1 2 3 4
5 72 6 4 0 16 13 10 7 1 2 3 4
1 70 2 1 72 71
5 73 7 4 0 16 13 10 7 1 2 3 4
5 74 8 4 0 16 13 10 7 1 2 3 4
1 70 2 1 74 73
5 76 9 4 0 16 13 10 7 1 2 3 4
1 75 1 1 76
5 78 8 4 0 16 13 10 7 1 2 3 4
1 77 1 1 78
5 80 7 4 0 16 13 10 7 1 2 3 4
1 79 1 1 80
5 82 6 4 0 16 13 10 7 1 2 3 4
1 81 1 1 82
5 84 6 4 0 16 13 10 7 1 2 3 4
1 83 1 0 84
5 86 7 4 0 16 13 10 7 1 2 3 4
1 85 1 0 86
5 88 6 4 0 16 13 10 7 1 2 3 4
1 87 1 0 88
5 90 7 4 0 16 13 10 7 1 2 3 4
1 89 1 0 90
5 92 6 4 0 16 13 10 7 1 2 3 4
5 93 7 4 0 16 13 10 7 1 2 3 4
1 91 2 1 93 92
5 95 5 4 0 16 13 10 7 1 2 3 4
5 96 6 4 0 16 13 10 7 1 2 3 4
1 94 2 1 96 95
5 97 7 4 0 16 13 10 7 1 2 3 4
5 98 8 4 0 16 13 10 7 1 2 3 4
1 94 2 1 98 97
5 100 6 4 0 16 13 10 7 1 2 3 4
1 99 1 0 100
5 102 7 4 0 16 13 10 7 1 2 3 4
1 101 1 0 102
5 104 6 4 0 16 13 10 7 1 2 3 4
1 103 1 0 104
5 106 7 4 0 16 13 10 7 1 2 3 4
1 105 1 0 106
5 108 9 4 0 16 13 10 7 1 2 3 4
1 107 1 1 108
5 110 8 4 0 16 13 10 7 1 2 3 4
1 109 1 1 110
5 112 7 4 0 16 13 10 7 1 2 3 4
1 111 1 1 112
5 114 6 4 0 16 13 10 7 1 2 3 4
1 113 1 1 114
5 116 6 4 0 16 13 10 7 1 2 3 4
5 117 7 4 0 16 13 10 7 1 2 3 4
1 115 2 1 117 116
5 119 5 4 0 16 13 10 7 1 2 3 4
5 120 6 4 0 16 13 10 7 1 2 3 4
1 118 2 1 120 119
5 121 7 4 0 16 13 10 7 1 2 3 4
5 122 8 4 0 16 13 10 7 1 2 3 4
1 118 2 1 122 121
5 124 6 4 0 16 13 10 7 1 2 3 4
1 123 1 0 124
5 126 7 4 0 16 13 10 7 1 2 3 4
1 125 1 0 126
5 128 6 4 0 16 13 10 7 1 2 3 4
1 127 1 0 128
5 130 7 4 0 16 13 10 7 1 2 3 4
1 129 1 0 130
5 132 7 4 0 16 13 10 7 1 2 3 4
1 131 1 1 132
5 134 6 4 0 16 13 10 7 1 2 3 4
1 133 1 1 134
5 136 7 4 0 16 13 10 7 1 2 3 4
1 135 1 1 136
5 138 6 4 0 16 13 10 7 1 2 3 4
1 137 1 1 138
5 140 6 4 0 16 13 10 7 1 2 3 4
5 141 7 4 0 16 13 10 7 1 2 3 4
1 139 2 1 141 140
5 143 5 4 0 16 13 10 7 1 2 3 4
5 144 6 4 0 16 13 10 7 1 2 3 4
1 142 2 1 144 143
5 145 7 4 0 16 13 10 7 1 2 3 4
5 146 8 4 0 16 13 10 7 1 2 3 4
1 142 2 1 146 145
5 148 6 4 0 16 13 10 7 1 2 3 4
1 147 1 0 148
5 150 7 4 0 16 13 10 7 1 2 3 4
1 149 1 0 150
5 152 6 4 0 16 13 10 7 1 2 3 4
1 151 1 0 152
5 154 7 4 0 16 13 10 7 1 2 3 4
1 153 1 0 154
5 156 7 4 0 16 13 10 7 1 2 3 4
1 155 1 1 156
5 158 6 4 0 16 13 10 7 1 2 3 4
1 157 1 1 158
5 160 7 4 0 16 13 10 7 1 2 3 4
1 159 1 1 160
5 162 6 4 0 16 13 10 7 1 2 3 4
1 161 1 1 162
5 164 4 4 0 16 13 10 7 1 2 3 4
5 165 7 4 0 16 13 10 7 1 2 3 4
1 163 2 1 165 164
5 167 4 4 0 16 13 10 7 1 2 3 4
5 168 6 4 0 16 13 10 7 1 2 3 4
1 166 2 1 168 167
5 169 5 4 0 16 13 10 7 1 2 3 4
5 170 7 4 0 16 13 10 7 1 2 3 4
1 166 2 1 170 169
5 172 4 4 0 16 13 10 7 1 2 3 4
5 173 7 4 0 16 13 10 7 1 2 3 4
1 171 2 1 173 172
5 175 4 4 0 16 13 10 7 1 2 3 4
5 176 6 4 0 16 13 10 7 1 2 3 4
1 174 2 1 176 175
5 177 5 4 0 16 13 10 7 1 2 3 4
5 178 7 4 0 16 13 10 7 1 2 3 4
1 174 2 1 178 177
5 180 4 4 0 16 13 10 7 1 2 3 4
5 181 7 4 0 16 13 10 7 1 2 3 4
1 179 2 1 181 180
5 183 4 4 0 16 13 10 7 1 2 3 4
5 184 6 4 0 16 13 10 7 1 2 3 4
1 182 2 1 184 183
5 185 5 4 0 16 13 10 7 1 2 3 4
5 186 7 4 0 16 13 10 7 1 2 3 4
1 182 2 1 186 185
5 188 4 4 0 16 13 10 7 1 2 3 4
5 189 7 4 0 16 13 10 7 1 2 3 4
1 187 2 1 189 188
5 191 5 4 0 16 13 10 7 1 2 3 4
5 192 8 4 0 16 13 10 7 1 2 3 4
1 190 2 1 192 191
5 194 4 4 0 16 13 10 7 1 2 3 4
5 195 7 4 0 16 13 10 7 1 2 3 4
1 193 2 1 195 194
5 197 5 4 0 16 13 10 7 1 2 3 4
5 198 8 4 0 16 13 10 7 1 2 3 4
1 196 2 1 198 197
5 200 4 4 0 16 13 10 7 1 2 3 4
5 201 7 4 0 16 13 10 7 1 2 3 4
1 199 2 1 201 200
5 203 5 4 0 16 13 10 7 1 2 3 4
5 204 8 4 0 16 13 10 7 1 2 3 4
1 202 2 1 204 203
5 206 6 4 0 16 13 10 7 1 2 3 4
5 207 7 4 0 16 13 10 7 1 2 3 4
1 205 2 1 207 206
5 209 5 4 0 16 13 10 7 1 2 3 4
5 210 6 4 0 16 13 10 7 1 2 3 4
1 208 2 1 210 209
5 211 7 4 0 16 13 10 7 1 2 3 4
5 212 8 4 0 16 13 10 7 1 2 3 4
1 208 2 1 212 211
5 214 6 4 0 16 13 10 7 1 2 3 4
5 215 7 4 0 16 13 10 7 1 2 3 4
1 213 2 1 215 214
5 217 5 4 0 16 13 10 7 1 2 3 4
5 218 6 4 0 16 13 10 7 1 2 3 4
1 216 2 1 218 217
5 219 7 4 0 16 13 10 7 1 2 3 4
5 220 8 4 0 16 13 10 7 1 2 3 4
1 216 2 1 220 219
5 222 5 4 0 16 13 10 7 1 2 3 4
5 223 7 4 0 16 13 10 7 1 2 3 4
1 221 2 1 223 222
5 225 4 4 0 16 13 10 7 1 2 3 4
5 226 6 4 0 16 13 10 7 1 2 3 4
1 224 2 1 226 225
5 228 6 4 0 16 13 10 7 1 2 3 4
5 229 8 4 0 16 13 10 7 1 2 3 4
1 227 2 1 229 228
5 231 7 4 0 16 13 10 7 1 2 3 4
5 232 9 4 0 16 13 10 7 1 2 3 4
1 230 2 1 232 231
5 234 6 4 0 16 13 10 7 1 2 3 4
5 235 7 4 0 16 13 10 7 1 2 3 4
1 233 2 1 235 234
5 237 5 4 0 16 13 10 7 1 2 3 4
5 238 6 4 0 16 13 10 7 1 2 3 4
1 236 2 1 238 237
5 239 7 4 0 16 13 10 7 1 2 3 4
5 240 8 4 0 16 13 10 7 1 2 3 4
1 236 2 1 240 239
5 242 5 4 0 16 13 10 7 1 2 3 4
5 243 7 4 0 16 13 10 7 1 2 3 4
1 241 2 1 243 242
5 245 4 4 0 16 13 10 7 1 2 3 4
5 246 6 4 0 16 13 10 7 1 2 3 4
1 244 2 1 246 245
5 248 6 4 0 16 13 10 7 1 2 3 4
5 249 8 4 0 16 13 10 7 1 2 3 4
1 247 2 1 249 248
5 251 7 4 0 16 13 10 7 1 2 3 4
5 252 9 4 0 16 13 10 7 1 2 3 4
1 250 2 1 252 251
5 254 6 4 0 16 13 10 7 1 2 3 4
5 255 7 4 0 16 13 10 7 1 2 3 4
1 253 2 1 255 254
5 257 5 4 0 16 13 10 7 1 2 3 4
5 258 6 4 0 16 13 10 7 1 2 3 4
1 256 2 1 258 257
5 259 7 4 0 16 13 10 7 1 2 3 4
5 260 8 4 0 16 13 10 7 1 2 3 4
1 256 2 1 260 259
5 262 5 4 0 16 13 10 7 1 2 3 4
5 263 7 4 0 16 13 10 7 1 2 3 4
1 261 2 1 263 262
5 265 4 4 0 16 13 10 7 1 2 3 4
5 266 6 4 0 16 13 10 7 1 2 3 4
1 264 2 1 266 265
5 268 6 4 0 16 13 10 7 1 2 3 4
5 269 8 4 0 16 13 10 7 1 2 3 4
1 267 2 1 269 268
5 271 7 4 0 16 13 10 7 1 2 3 4
5 272 9 4 0 16 13 10 7 1 2 3 4
1 270 2 1 272 271
5 274 6 4 0 16 13 10 7 1 2 3 4
5 275 7 4 0 16 13 10 7 1 2 3 4
1 273 2 1 275 274
5 277 5 4 0 16 13 10 7 1 2 3 4
5 278 6 4 0 16 13 10 7 1 2 3 4
1 276 2 1 278 277
5 279 8 4 0 16 13 10 7 1 2 3 4
5 280 9 4 0 16 13 10 7 1 2 3 4
1 276 2 1 280 279
0
113 ouch20
118 ouch21
94 ouch16
125 ouch22
101 ouch17
129 ouch23
105 ouch18
83 okay14
109 ouch19
196 ouch36
3 c(1)
4 c(2)
5 c(3)
6 c(4)
224 ouch40
216 ouch39
182 ouch34
174 ouch33
190 ouch35
208 ouch38
87 okay15
75 okay12
79 okay13
159 okay31
35 okay4
202 ouch37
39 okay5
43 okay6
163 okay32
151 okay29
155 okay30
131 okay24
51 okay7
139 okay26
147 okay28
135 okay25
247 okay44
270 ouch47
55 okay8
63 okay10
241 okay43
67 okay11
2 p
59 okay9
233 okay42
227 okay41
261 okay46
27 okay2
31 okay3
253 okay45
111 okay20
115 okay21
91 okay16
123 okay22
7 d(4)
10 d(3)
13 d(2)
16 d(1)
99 okay17
127 okay23
8 n_d(4)
11 n_d(3)
14 n_d(2)
17 n_d(1)
103 okay18
107 okay19
193 okay36
221 okay40
85 ouch14
213 okay39
187 okay35
179 okay34
77 ouch12
171 okay33
81 ouch13
205 okay38
89 ouch15
37 ouch4
161 ouch31
41 ouch5
199 okay37
153 ouch29
46 ouch6
157 ouch30
166 ouch32
133 ouch24
53 ouch7
137 ouch25
142 ouch27
250 ouch44
149 ouch28
273 okay48
267 okay47
57 ouch8
244 ouch43
19 okay1
276 ouch48
65 ouch10
61 ouch9
70 ouch11
236 ouch42
22 ouch1
230 ouch41
264 ouch46
256 ouch45
29 ouch2
33 ouch3
0
B+
0
B-
1
0
1
"""
output = """
{p, c(1), c(2), c(3), c(4), d(4), d(3), d(2), d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), d(4), d(3), d(2), n_d(1), okay4, ouch4, okay5, ouch5, okay9, ouch9, okay10, ouch10, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29}
{p, c(1), c(2), c(3), c(4), d(4), d(3), n_d(2), d(1), okay2, okay4, ouch4, okay5, ouch5, okay7, okay9, ouch9, okay10, ouch10, okay12, okay14, ouch14, okay15, ouch15, okay17, ouch17, okay18, ouch18, okay19, okay22, ouch22, okay23, ouch23, okay28, ouch28, okay29, ouch29, ouch41, ouch44, ouch47, ouch48}
{p, c(1), c(2), c(3), c(4), d(4), d(3), n_d(2), n_d(1), ouch1, okay2, ouch2, okay4, ouch4, okay5, ouch5, ouch6, okay7, ouch7, okay9, ouch9, okay10, ouch10, ouch11, okay12, ouch12, okay14, ouch14, okay15, ouch15, ouch16, okay17, ouch17, okay18, ouch18, okay19, ouch19, ouch21, okay22, ouch22, okay23, ouch23, ouch27, okay28, ouch28, okay29, ouch29, ouch35, ouch36, ouch37, ouch38, ouch39, okay41, ouch41, ouch42, okay44, ouch44, ouch45, okay47, ouch47}
{p, c(1), c(2), c(3), c(4), d(4), n_d(3), d(2), d(1), ouch1, okay2, ouch2, okay4, ouch4, okay5, ouch5, ouch6, okay7, ouch7, okay9, ouch9, okay10, ouch10, ouch11, okay12, ouch12, okay14, ouch14, okay15, ouch15, ouch16, okay17, ouch17, okay18, ouch18, okay19, ouch19, ouch21, okay22, ouch22, okay23, ouch23, ouch27, okay28, ouch28, okay29, ouch29, ouch35, ouch36, ouch37, ouch38, ouch39, okay41, ouch41, ouch42, okay44, ouch44, ouch45, okay47, ouch47}
{p, c(1), c(2), c(3), c(4), n_d(4), d(3), d(2), d(1), okay1, okay2, ouch2, okay3, okay4, okay5, okay6, okay7, ouch7, okay8, okay9, okay10, okay11, okay12, ouch12, okay13, okay14, okay15, okay16, okay17, okay18, okay19, ouch19, okay20, okay21, okay22, okay23, okay24, okay25, okay26, okay28, okay29, okay30, okay31, okay32, ouch32, okay33, ouch33, okay34, ouch34, okay35, ouch35, okay36, ouch36, okay37, ouch37, okay38, okay39, okay40, okay41, okay42, okay43, okay44, okay45, okay46, okay47, okay48}
{p, c(1), c(2), c(3), c(4), n_d(4), d(3), d(2), n_d(1), ouch1, okay2, ouch2, okay3, ouch3, ouch6, okay7, ouch7, okay8, ouch8, ouch11, okay12, ouch12, okay13, ouch13, ouch16, okay19, ouch19, okay20, ouch20, ouch21, okay24, ouch24, okay25, ouch25, ouch27, okay30, ouch30, okay31, ouch31, okay32, ouch32, okay33, ouch33, okay34, ouch34, okay35, ouch35, okay36, ouch36, okay37, ouch37, ouch38, ouch39, okay40, ouch40, ouch42, okay43, ouch43, ouch45, okay46, ouch46, ouch48}
{p, c(1), c(2), c(3), c(4), n_d(4), d(3), n_d(2), d(1), okay2, ouch2, okay3, ouch3, okay7, ouch7, okay8, ouch8, okay12, ouch12, okay13, ouch13, okay19, ouch19, okay20, ouch20, okay24, ouch24, okay25, ouch25, okay30, ouch30, okay31, ouch31, okay32, ouch32, okay33, ouch33, okay34, ouch34, okay35, okay36, okay37, ouch40, ouch43, ouch46}
{p, c(1), c(2), c(3), c(4), n_d(4), d(3), n_d(2), n_d(1), okay2, ouch2, okay3, ouch3, okay7, ouch7, okay8, ouch8, okay12, ouch12, okay13, ouch13, okay19, ouch19, okay20, ouch20, okay24, ouch24, okay25, ouch25, okay30, ouch30, okay31, ouch31}
{p, c(1), c(2), c(3), c(4), n_d(4), n_d(3), d(2), d(1), okay2, ouch2, okay3, ouch3, okay7, ouch7, okay8, ouch8, okay12, ouch12, okay13, ouch13, okay19, ouch19, okay20, ouch20, okay24, ouch24, okay25, ouch25, okay30, ouch30, okay31, ouch31}
{p, c(1), c(2), c(3), c(4), n_d(4), n_d(3), d(2), n_d(1), okay2, ouch2, okay3, ouch3, okay7, ouch7, okay8, ouch8, okay12, ouch12, okay13, ouch13, okay19, ouch19, okay20, ouch20, okay24, ouch24, okay25, ouch25, okay30, ouch30, okay31, ouch31}
{p, c(1), c(2), c(3), c(4), d(4), n_d(3), d(2), n_d(1), okay1, okay2, ouch2, okay3, okay4, okay5, okay6, okay7, ouch7, okay8, okay9, okay10, okay11, okay12, ouch12, okay13, okay14, okay15, okay16, okay17, okay18, okay19, ouch19, okay20, okay21, okay22, okay23, okay24, okay25, okay26, okay28, okay29, okay30, okay31, okay32, ouch32, okay33, ouch33, okay34, ouch34, okay35, ouch35, okay36, ouch36, okay37, ouch37, okay38, okay39, okay40, okay41, okay42, okay43, okay44, okay45, okay46, okay47, okay48}
{p, c(1), c(2), c(3), c(4), n_d(4), n_d(3), n_d(2), d(1), okay2, ouch2, okay3, ouch3, okay7, ouch7, okay8, ouch8, okay12, ouch12, okay13, ouch13, okay19, ouch19, okay20, ouch20, okay24, ouch24, okay25, ouch25, okay30, ouch30, okay31, ouch31}
{p, c(1), c(2), c(3), c(4), n_d(4), n_d(3), n_d(2), n_d(1), okay2, ouch2, okay3, ouch3, okay7, ouch7, okay8, ouch8, okay12, ouch12, okay13, ouch13, okay19, ouch19, okay20, ouch20, okay24, ouch24, okay25, ouch25, okay30, ouch30, okay31, ouch31}
{p, c(1), c(2), c(3), c(4), d(4), n_d(3), n_d(2), d(1), ouch1, okay2, ouch2, okay3, ouch3, ouch6, okay7, ouch7, okay8, ouch8, ouch11, okay12, ouch12, okay13, ouch13, ouch16, okay19, ouch19, okay20, ouch20, ouch21, okay24, ouch24, okay25, ouch25, ouch27, okay30, ouch30, okay31, ouch31, okay32, ouch32, okay33, ouch33, okay34, ouch34, okay35, ouch35, okay36, ouch36, okay37, ouch37, ouch38, ouch39, okay40, ouch40, ouch42, okay43, ouch43, ouch45, okay46, ouch46, ouch48}
{p, c(1), c(2), c(3), c(4), d(4), n_d(3), n_d(2), n_d(1), okay2, ouch2, okay3, ouch3, okay7, ouch7, okay8, ouch8, okay12, ouch12, okay13, ouch13, okay19, ouch19, okay20, ouch20, okay24, ouch24, okay25, ouch25, okay30, ouch30, okay31, ouch31, okay32, ouch32, okay33, ouch33, okay34, ouch34, okay35, okay36, okay37, ouch40, ouch43, ouch46}
"""
