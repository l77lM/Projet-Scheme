; TEST_RETURN_CODE=PASS
; TEST_COMMENT=Test if basic

(if #t 1 2)
(if #f 1 2)
(if #t 1)
(if #f 1)

;;; Make sure you put a return at
;;; the end of the input file
