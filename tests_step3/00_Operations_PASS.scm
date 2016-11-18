;;;;;;;;;;;;;;;; Script de test ;;;;;;;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT= test opérations
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 

(define a -9)
(+ 3 4)
(+ 3)
(+)
(+ a 5)
(* 4)
(*)
(* 6 a)
(- 3 4)
(- 3 4 5)
(- 3)
(- 8 a)
(/ 3 4 5)
(/ 3)
(/ a 3)

(abs -7)
(abs a)

(remainder 13 4)
(remainder -13 4)
(remainder 13 -4)
(remainder -13 -4)
(remainder a 2)

(= 1 1)
(= 2 3)
(= 3 6 7)
(= a 9)
(< 1 2)
(< 2 3 6)
(< 3 2 6)
(< a 15)
(> 3 6)
(> 9 5 1)
(> 9 10)
(> a 15)


