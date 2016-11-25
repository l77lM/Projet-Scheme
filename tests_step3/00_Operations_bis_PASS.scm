;;;;;;;;;;;;;;;; Script de test ;;;;;;;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT= test opérations plus difficiles 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 

(+ +inf 3)
(- +inf 3)
(+ -inf 3)
(- -inf 3)
(+ +inf -inf)
(+ -inf +inf)

(+ 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 )
(+ 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 +inf )
(+ 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 -inf )

(* +inf 3)
(* -inf 3)
(* +inf -3)
(* -inf -3)

(/ +inf 3)
(/ -inf 3)
(/ +inf -3)
(/ -inf -3)

(/ +inf -inf)
(/ +inf +inf)
(/ -inf +inf)
(/ -inf -inf)

(= +inf -inf)
(= +inf +inf)
(= -inf -inf)

(> 2 +inf)
(> +inf 2)
(< 2 +inf)
(< +inf 2) 

(> 2 -inf)
(> -inf 2)
(< 2 -inf)
(< -inf 2)

