Data file consists of succesively dumped vectors and matrices by boost::uBLAS.

c^T:
  [6](4,-3,0,-1,1,0)
b^T:
  [4](13,26,1,0)
A:
  [4,6]((-1,3,0,1,0,0),(4,1,0,0,1,0),(-2,1,1,0,0,0),(1,-3,0,0,0,1))
Variables signs: -1, 0 or 1 accordingly for x_i <= 0, x_i has any sign or x_i >= 0.
  [6](1,1,1,1,1,1)
Constraint symbol: -1, 0 or 1 accordingly for A_i * x <= b_i, A_i * x = b_i or A_i * x >= b_i.
  [6](1,1,1,1,1,1)

Program solves: min (c^T * x), 
where x satisfies A * x >=< b, with according to above sign on '>=<' place 
and x has according to above sign limitation.
