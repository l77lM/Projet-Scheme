;;;;;;;;;;;;;;;; Script de test ;;;;;;;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT= test Conversion
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 

(integer->char 1)
(integer->char 36)
(integer->char 127)

(char->integer #\a)
(char->integer #\space)
(char->integer #\8)

(string->number "1")
(string->number "128")
(string->number "1.92")

(number->string 1)
(number->string 128)
(number->string 1.92)

(symbol->string 'a)
(symbol->string 'abcd)

(string->symbol "a")
(string->symbol "abcd")







