> CILisp

> By: Francisco Funes Alfaro

> COMP 232L - Fall 2019

> TASK 1: The first set of sample runs will show that each individual function for Task 1 works on it's own with both doubles and integers. Produce the correct results, and they also work nested within each other as the Cambridge Polish Notation Dictates.
>
    // negation
    > (neg -1)
    1
    > (neg -1.0)
    1.00
    > (neg (neg -1))
    -1
    > (neg (neg (neg -1.25)))
    1.25
>
    // absolute value
    > (abs -10)
    10
    > (abs -12.25)
    12.25
    > (abs 25)
    25
    > (abs -25)
    25
    > (abs (abs -25))
    25
    > (abs (abs (abs -25.526)))
    25.53
>
    // e^power, exp
    > (exp 1)
    2
    > (exp 1.0) // more precision with the decimal point
    2.72
    > (exp 2)
    7
    > (exp 2.0)
    7.39
    > (exp 2.5)
    12.18
    > (exp (exp 1))
    15
    > (exp (exp 1.0))
    15.15
>
    // square root
    > (sqrt 4)
    2
    > (sqrt (sqrt 16))
    2
    > (sqrt (sqrt 16.0))
    2.00
    > (sqrt 2.5)
    1.58
    > (sqrt (sqrt (sqrt 81)))
    1
    > (sqrt (sqrt (sqrt 81.0)))
    1.73
>
    // natural log
    > (log 100)
    4
    > (log 100.0)
    4.61
    > (log (log 125.234))
    1.57
>
    // 2^input
    > (exp2 1.5)
    2.83
    > (exp2 2)
    4
    > (exp2 2.0)
    4.00
    > (exp2 (exp2 3))
    256
    > (exp2 -2)
    0
    > (exp2 -2.0)
    0.25
> 
    //cube root of input
    > (cbrt 8)
    2
    > (cbrt 32.0)
    3.17
    > (cbrt (cbrt 32))
    1
    > (cbrt (cbrt 32.0))
    1.47
    
>   
    // two operand inputs
    // subtraction of two inputs
    > (sub 5 1)
    4
    > (sub 1 5)
    -4
    > (sub -5 5)
    -10
    > (sub 12 -30)
    42
    > (sub 12.5 3) // when one op is double
    9.50
    > (sub 12 3.5) // when the other op is double
    8.50
    > (sub -12 3.5)
    -15.50
    > (sub -12.5 3)
    -15.50
    > (sub -12 -12)
    0
    > (sub -12.5 -12.6) 
    0.10
>
    // division of two inputs
    > (div 1 4)
    0
    > (div 1 4.0) 
    0.25
    > (div -1 5.0)
    -0.20
    > (div 20 5)
    4
    > (div 20.523 5)
    4.10
> 
    // remainder
    > (remainder 4 2)
    0
    > (remainder 5 2)
    1
    > (remainder 5.5 2)
    1.50
    > (remainder 5 2.0)
    1.00
    > (remainder -5 2.0)
    -1.00
    > (remainder 10 2)
    0
>
    // one operand to the power of the other
    > (pow 2 3)
    8
    > (pow 2 3.5)
    11.31
    > (pow -2 2)
    4
    > (pow -2.5 -2.0)
    0.16
>
    // max
    > (max 5 10)
    10
    > (max -5 -10)
    -5
    > (max -5.5 -5.4)
    -5.40
    > (max -5 5.4)
    5.40
>
    // min
    > (min -5.5 -5.4)
    -5.50
    > (min -10 5)
    -10
    > (min -10 -10.1)
    -10.10
>
    // hypotenuse of two inputs
    > (hypot 2 2)
    2
    > (hypot 2.0 2)
    2.83
    > (hypot 4 5.0)
    6.40
    > (hypot 4.0 5.0)
    6.40
>
    // add
    > (add 1 1)
    2
    > (add 1 1.5)
    2.50
    > (add 1.5 1)
    2.50
    > (add 1.5 1.5)
    3.00
    > (add 1 2 3.5)
    6.50
    > (add -1.5 2.5 3) 
    4.00
    > (add 1 2 3)
    6
    > (add 1 2.5 3) // it only takes one double to promote result
    6.50
>
    // mult
    > (mult 2 2 2.0)
    8.00
    > (mult 2 2 2)
    8
    > (mult 2 16 32)
    1024
    > (mult 1 2 3 4 5 6)
    720
    > (mult -2 2)
    -4
    > (mult -2.5 2)
    -5.00
    > (mult 2.5 -2)
    -5.00
    > (mult 2 2.5)
    5.00
>
    // nested runs
    // programmatically, every case runs with the 
    // same code, literally copy and paste,
    // and since all of them produced the right results
    // on their own, then nesting them shouldn't be a problem.
    > (neg (add (mult 1 2) (div 20 5)))
    -6
    > (add (neg (mult 2.5 2)) (sub (add 2 2) (div 20 5.3)))
    -4.77
> There are a couple of issues I ran into. The first was that for some of the one operand functions, if I input integers, the result might lose precision because it's a decimal, but because I input an int type, there is no casting done. I could have fixed this by printing an error, or changing the type for these functions.

> Another issue I could possibly run into is that I didn't test every possible nested case, for every function. So further debugging might be required.
 
> Another issue is that if I input negative numbers into the square or cube root functions, the result would be imaginary, so even though something prints, it's wrong. I could fix this by checking that the input is greater than or equal to zero for these functions, and execute. Else use default one and print an error message.

> What I tested was that every function produce a value of the same type for the one op ones, if I put in an int, an int is returned. And the same with a double. 

> For two op functions I tested that if one op was a double, that the result would be promoted, or if the other was a double, the result would be promoted. If both were ints, the default for result is an int type, so no need to do any checking. 

> For any result that I was unsure of, I tossed the input into a calculator to check, so at least every sample run above produced the correct results. 
  