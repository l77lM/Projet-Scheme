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

(integer? 3.0)
(integer? (/ 8 4))
(integer? (+ 5 1))
(integer? +inf)
(integer? #t)
(integer? (/ 8 5))
(integer? 1.8)

(char? 1)
(char? #t)
(char? #\4)
(char? 'a)
(char? '())

(null? 1)
(null? '())
(null? 'a)
(null? #\a)
(null? ())
(null? #t)

(string? 'a)
(string? 45)
(string? "")
(string? "abc")
(string? ())
(string? #t)
(string? #\a)

(pair? 'a)
(pair? 45)
(pair? "abc")
(pair? ())
(pair? #t)
(pair? #\a)
(pair? '(1 2))
(pair? (cons 1 2))
(pair? '(("bonjour") ("bonjour") ("bonjour")))

(real? 'a)
(real? 45)
(real? "abc")
(real? ())
(real? #t)
(real? #\a)
(real? '(1 2))
(real? 1.5)
(real? (/ 8 4))
(real? (/ 8 5))

(pinfty? 'a)
(pinfty? 45)
(pinfty? "abc")
(pinfty? ())
(pinfty? #t)
(pinfty? #\a)
(pinfty? '(1 2))
(pinfty? 1.5)
(pinfty? +inf)
(pinfty? -inf)

(minfty? 'a)
(minfty? 45)
(minfty? "abc")
(minfty? ())
(minfty? #t)
(minfty? #\a)
(minfty? '(1 2))
(minfty? 1.5)
(minfty? +inf)
(minfty? -inf)
