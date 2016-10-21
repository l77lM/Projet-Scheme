; TEST_RETURN_CODE=PASS
; TEST_COMMENT=Test read basic chars

(+ 1 2)
(+ +1 +2)
(+ -1 +2)
(+ 1 -2)
(+ 1 1 1 1 1 1)

(- 1 2)
(- 2 1)
(- 1 -1)
(- 3 1 1 1)
(- 0 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1)

(* 1 2)
(* 2 1)
(* -1 2)
(* -2 1)
(* 2 1 1 1 1 1 1 1 1)

(/ 2 1)
(/ -2 1)
(/ 2 -1)
(/ 64 2 2 2 2 2 2)
	
(/ (+ 10 30 2) (* 3 (- 13 6)))
;;; Make sure you put a return at
;;; the end of the input file
