;;;;;;;;;;;;;;;; Script de test ;;;;;;;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT= test predicat
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 

(boolean? #f)
(boolean? 0)
(boolean? '())

(symbol? 'foo)
(symbol? (car '(a b)))
(symbol? "bar")
(symbol? 'nil)
(symbol? '())
(symbol? #f)

(integer? 3+0i)
(integer? 3.0)
(integer? 8/4)
