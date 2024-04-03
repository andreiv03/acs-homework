#lang racket
(require "suffix-tree-stream.rkt")
(require "collection.rkt")

(provide (all-defined-out))

;; Vom prelua toate funcțiile din etapele 1-3 (exceptând
;; longest-common-substring, care nu beneficiază de 
;; reprezentarea ca flux întrucât parcurge tot arborele)
;; și le vom adapta la noua reprezentare a unui ST.
;;
;; Pentru că un ST este construit pornind de la o colecție
;; de sufixe și pentru că ne dorim să nu calculăm toate
;; sufixele decât dacă este nevoie, vom modifica toate
;; funcțiile care prelucrau liste de sufixe pentru a
;; prelucra fluxuri de sufixe.
;;
;; Obs: fără această modificare a listelor de sufixe în
;; fluxuri de sufixe, și presupunând că am manipulat
;; arborii de sufixe doar prin interfața definită în
;; fișierul suffix-tree (respectând astfel bariera de 
;; abstractizare), ar trebui să alterăm doar funcția 
;; suffixes->st care este practic un constructor pentru
;; tipul ST.
;; Din cauza transformării listelor de sufixe în fluxuri,
;; avem mult mai multe implementări de modificat.
;; Puteam evita acest lucru? Da, utilizând conceptul de
;; colecție de sufixe de la început (în loc să presupunem
;; că ele vor fi prelucrate ca liste). În loc de cons,
;; car, cdr, map, filter, etc. am fi folosit de fiecare
;; dată collection-cons, collection-first, ... etc. -
;; aceste funcții fiind definite într-o bibliotecă
;; inițială ca fiind echivalentele lor pe liste, și
;; redefinite ulterior în stream-cons, stream-first,
;; ... etc. Operatorii pe colecții de sufixe ar fi 
;; folosit, desigur, doar funcții de tip collection-.
;;
;; Am ales să nu procedăm astfel pentru că ar fi provocat
;; confuzie la momentul respectiv (când chiar operatorii
;; pe liste erau o noutate) și pentru a vă da ocazia să
;; faceți singuri acest "re-design".


; TODO
; Copiați din etapele anterioare implementările funcțiilor
; de mai jos și modificați-le astfel:
; - Toate funcțiile care lucrează cu liste de sufixe vor
;   lucra cu un nou tip de date Collection, ai cărui
;   constructori și operatori vor fi definiți de voi
;   în fișierul collection.rkt.
; - Pentru toate funcțiile, trebuie să vă asigurați că
;   este respectată bariera de abstractizare (atât în 
;   cazul tipului ST cât și în cazul tipului Collection).
; Obs: cu cât mai multe funcții rămân nemodificate, cu atât
; este mai bine (înseamnă că design-ul inițial a fost bun).
(define (longest-common-prefix w1 w2)
  (define (helper w1 w2 lcp)
    (if (or (null? w1) (null? w2))
      (list (reverse lcp) w1 w2)
      (if (equal? (car w1) (car w2))
        (helper (cdr w1) (cdr w2) (cons (car w1) lcp))
        (list (reverse lcp) w1 w2))))

  (helper w1 w2 '()))


; am schimbat, în numele funcției, cuvântul list în
; cuvântul collection
(define (longest-common-prefix-of-collection words)
  (define (helper words lcpol)
    (if (collection-empty? words)
      lcpol
      (helper (collection-rest words) (collection-first (longest-common-prefix (collection-first words) lcpol)))))

  (helper (collection-rest words) (collection-first words)))


(define (match-pattern-with-label st pattern)
  (let ([branch (get-ch-branch st (car pattern))])
    (if (equal? branch #f)
      (list #f '())
      (let* ([label (get-branch-label branch)] [lcp (longest-common-prefix pattern label)])
        (if (equal? (car lcp) pattern)
          #t
          (if (equal? (car lcp) label)
            (list label (drop pattern (length label)) (get-branch-subtree branch))
            (list #f (car lcp))))))))


(define (st-has-pattern? st pattern)
  (let ([branch (get-ch-branch st (car pattern))])
    (if (equal? branch #f)
      #f
      (let ([label (get-branch-label branch)])
        (if (< (length label) (length pattern))
          (st-has-pattern? (get-branch-subtree branch) (drop pattern (length label)))
          (let ([lcp (longest-common-prefix label pattern)])
            (if (equal? (car lcp) pattern)
              #t
              #f)))))))


(define (get-suffixes text)
  (if (collection-empty? text)
    empty-collection
    (collection-cons text (get-suffixes (collection-rest text)))))


(define (get-ch-words words ch)
  (collection-filter
    (λ (word)
      (and (not (collection-empty? word)) (equal? (collection-first word) ch)))
    words))


(define (ast-func suffixes)
  (cons (list (car (collection-first suffixes))) (collection-map cdr suffixes)))


(define (cst-func suffixes)
  (let ([lcpol (longest-common-prefix-of-collection suffixes)])
    (cons lcpol (collection-map
      (λ (suffix)
        (drop suffix (length lcpol)))
      suffixes))))


; considerați că și parametrul alphabet este un flux
; (desigur, și suffixes este un flux, fiind o colecție
; de sufixe)
(define (suffixes->st labeling-func suffixes alphabet)
  (collection-filter (λ (x) (pair? x))
    (collection-map
      (λ (ch)
        (let ([gcw (get-ch-words suffixes ch)])
          (if (collection-empty? gcw)
            empty-collection
            (let ([lf (labeling-func gcw)])
              (let ([st (suffixes->st labeling-func (cdr lf) alphabet)])
                (cons (car lf) st))))))
      alphabet)))


; nu uitați să convertiți alfabetul într-un flux
(define (text->st labeling-func)
  (λ (text)
    (let ([suffixes (get-suffixes (append text '(#\$)))])
      (let ([alphabet (list->stream (sort (remove-duplicates (append text '(#\$))) char<?))])
        (suffixes->st labeling-func suffixes alphabet)))))


(define text->ast
  (text->st ast-func))


(define text->cst
  (text->st cst-func))


; dacă ați respectat bariera de abstractizare,
; această funcție va rămâne nemodificată.
(define (substring? text pattern)
  (st-has-pattern? ((text->st ast-func) text) pattern))


; dacă ați respectat bariera de abstractizare,
; această funcție va rămâne nemodificată.
(define (repeated-substring-of-given-length text len)
  (let loop-over-branches ([st ((text->st cst-func) text)] [rsogl '()] [label '()] [is-subtree #f])
    (if (collection-empty? st)
      (let* ([appended (append rsogl label)])
        (if (and (not (equal? is-subtree #f)) (not (empty? appended)))
          rsogl
          (if (empty? appended)
            #f
            appended)))

      (let* ([subtree-rsogl (loop-over-branches (get-branch-subtree (first-branch st)) (append rsogl label) (get-branch-label (first-branch st)) #t)])
        (if (>= (length subtree-rsogl) len)
          (take subtree-rsogl len)
          (loop-over-branches (other-branches st) (append rsogl label) '() #f))))))

