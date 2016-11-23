;;;;;;;;;;;;;;;; Script de test ;;;;;;;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT= test equiv polymorphique
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 

(eq? 'a 'a)
(eq? (list 'a) (list 'a))
(eq? '() '())
(eq? car car)
(eq? '(1 2) '(1 2))

(eq? 2 2)
(eq? 2 3)

(eq? 1.5 1.5)
(eq? 1.5 1.4)
(eq? +inf +inf)
(eq? +inf -inf)
(eq? 4 +inf)

(eq? "" "")
(eq? "a" "a")
(eq? "a" "bc")

(eq? #\a #\a)
(eq? #\a #\4)





