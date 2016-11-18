;;;;;;;;;;;;;;;; Script de test ;;;;;;;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT= test equiv polymorphique
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 

(eq? 'a 'a)
(eq? (list 'a) (list 'a))
(eq? '() '())
(eq? car car)
(eq? 2 2)
(eq? 2 3)
(eq? "" "")
(eq? "a" "a")
(eq? "a" "bc")



