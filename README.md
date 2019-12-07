> CILisp

> By: Francisco Funes Alfaro

> COMP 232L - Fall 2019

> **TASK 1:** The first set of sample runs will show that each individual function for Task 1 works on it's own with both doubles and integers. Produce the correct results, and they also work nested within each other as the Cambridge Polish Notation Dictates.
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
    > (sub (neg -1) (abs -5.25)) // when both operands are a function
    -4.25
    > (sub (abs -5) 5) // when op1 is a function
    0
    > (sub 5 (abs -5.0)) // when op2 is a function
    0.00
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
    > (div (sub 25 5) (abs -5))
    4
    > (div 20 (abs -5))
    4
    > (div (abs -20.0) 5)
    4.00
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
    > (remainder (abs -10) (neg -2))
    0
    > (remainder 10.0 (abs -2))
    0.00
    > (remainder (abs -10) 2)
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
    > (pow (sub 4 2) 2)
    4
    > (pow 2 (sub 4.0 2))
    4.00
    > (pow (sub 4.0 2) (sub 4 2))
    4.00
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
    > (max (abs -10) (neg -20))
    20
    > (max 10 (neg -20.0))
    20.00
    > (max (neg -10.0) 20)
    20.00
    > (max (neg -20.0) 10)
    20.00
>
    // min
    > (min -5.5 -5.4)
    -5.50
    > (min -10 5)
    -10
    > (min -10 -10.1)
    -10.10
    > (min (neg -15.5) 2)
    2.00
    > (min 2.3 2.4)
    2.30
    > (min 2.3 (neg -2.2))
    2.20
    > (min (abs -10) (abs -25.34))
    10.00
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

> **TASK 2:** In this task, a token was added for symbol in the lex, the new productions were added to the yacc, and helper functions were added to the c code in order to construct the lists for variables.

>
    // Test from lab description
    > (add ((let (abcd 1)) (sub 3 abcd)) 4)
    6.00
    > (mult ((let (a 1) (b 2)) (add a b)) (sqrt 2))
    4.24
    > (add ((let (a ((let (b 2)) (mult b (sqrt 10))))) (div a 2)) ((let (c 5)) (sqrt c)))
    5.40
    > ((let (first (sub 5 1)) (second 2)) (add (pow 2 first) (sqrt second)))
    17.41
    > ((let (a ((let (c 3) (d 4)) (mult c d)))) (sqrt a))
    3.46
    // this run shows that variables can be set to doubles, and that the beginning and end of the list can be reached.
    > ((let (a 25.5) (b 33.56) (c 99.56))(add (mult b c) a))
    3366.73

> The runs from the description show that a let section can be used to define variables and even set a variable to the evaluation of a function.

> The runs also show that multi variable functionality works.

> I did not run into issues that affect functionality in my testing.

> **TASK 3:** The optional option of assigning a type to variables was added. So a variable could be an int or a double, and if a double tried to get assigned to an int, at evaluation time, precision was lost and a warning was printed.

>
    > ((let (int a 1.25))(add a 1))
    !!!Warning!!! Precision loss with assignment to a
    2
    > ((let (double a 35.23))(mult a 2)) // showing double type
    70.46
    > ((let (double a 2.5)(int b 3.3))(mult a b)) // a list of variables with type assigned
    !!!Warning!!! Precision loss with assignment to b
    7.50
    > (mult ((let (double a 2.5)(int b 3.3)) (add a b))(sqrt 2))
    !!!Warning!!! Precision loss with assignment to b
    7.78
    > ((let (a 2.5)(int b 3.3))(mult a b) ) // since the type was optional you can still define the same way as before
    !!!Warning!!! Precision loss with assignment to b
    7.50
    ((let (double a 2)(int b 3))(mult a b)) // no warning printed when int assigned to double, it's just promoted.
    6.00

> **TASK 4:** The print function was added, the lex got the "print" in the func token, and the case was filled in the C code.

>
    // runs
    > (print ((let (a 2.5)(b 3.5))(mult a b)) (div 20 5.5) (add 3.2 1))
    ==> 8.75 3.64 4.20
    4.20
    > (print 1) // one arg
    ==> 1
    1
    > (print (add 2 2) (sub 3 2)) // two arg with functions
    ==> 4 1
    1
    > (print ((let (a 2))a)) // an arg as a variable
    ==> 2.00
    2.00
    > (add (print 2) 5) // printing within a function
    ==> 2
    7
    > (add (print (mult 2 5) (add 2 2)) (sub 2 3))
    ==> 10 4
    3
    
> **TASK 5:** Functionality was expanded to support multi argument statements, I only modified the add mult and print as requested from lab. 
>
    > (add (add 1 2) (sub 4 2) (neg -20)) // nested funtions still work
    25
    > (add 1 (neg -2) 3)
    6
    > (add 1.0 2 (abs -25)) // it only takes one double to promote result
    28.00
    > ((let (int a 1)(double b 2))(print a b 3))
    ==> 1 2.00 3 
    3
    > (print (add 1 2) (sub 4.5 2) 5)
    ==> 3 2.50 5 
    5
    > (mult 1 2 3 4 5 6)
    720
    > (mult (add 1 0) (sub 2 0) 3 4 5)
    120
> **TASK 6:** New functions, read rand less greater equal were added. For read I can only take in 9 characters for a number, can read int and doubles. Issues with read include that if i input letters, the number embedded won't be read.
>
    > ((let (int a (read)) (double b (read)) (c (read)) (d (read))) (print a b c d))
    read := 3
    read := 5.0
    read := 10
    read := 5.175
    ==> 3 5.00 10.00 5.17 // ten is printed as a double, because i made the default type for symbol tables double if it wasn't specified.
    5.17
    > (less (rand) (rand))
    1.00
    > (less (rand) (rand))
    1.00
    > (less (rand) (rand))
    0.00
    > ((let (a 0)) (cond (less (rand) 0.5) (add a 1) (sub a 1)))
    -1.00
    > ((let (a 0)) (cond (less (rand) 0.5) (add a 1) (sub a 1))) // result changes with random number
    1.00
    > ((let (a 0)) (cond (less (rand) 0.5) (add a 1) (sub a 1)))
    -1.00
    > (rand)
    0.80
    > (rand)
    0.91
    > (rand)
    0.20
    > (equal 5 5)
    1
    > (equal 5 4)
    0
    > (equal (neg -5) (abs -5))
    1
    > (greater (neg -5) (add 5 1))
    0
    > (greater 5 (sub 4 1))
    1
    > (equal (sub 1 2) (sub 1 2))
    1
> **TASK 7:** Custom functions were added. For evaluation I went for the approach where first I evaluate and give arguments values, then I call eval on the definition for result. 
>
    > ((let (double myFunc lambda (x y) (mult (add x 5) (sub y 2)))) (sub (myFunc 3 5) 2))
    22.00
    > ((let (f lambda (x y) (add x y)))(f (sub 5 2) (mult 2 3)))
    9.00
    > ((let (f lambda (x) (add x 5)))(f (mult 2 3))) // double is default return type
    11.00
    > ((let (int f lambda (x) (add x 5)))(f (mult 2 3))) // return type was an int, int returned
    11
    > ((let (f lambda (x) (add x 5)))(f ))
    !!! ERROR !!! Please provide actual parameters on next attempt
    0
    > ((let (f lambda (x y) (add x y)))(f 5))
    !!! ERROR !!! too few operands were given, terminating...
    nan
    > ((let (f lambda (x y) (add x y)))(f 5 5 5))
    !!! WARNING !!! too many operands were given for the custom function
    10.00
    