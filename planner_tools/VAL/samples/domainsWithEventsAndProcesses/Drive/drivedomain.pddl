(define (domain drivedomain)
(:requirements :fluents :continuous-effects :negative-preconditions :duration-inequalities :time :typing)
(:types vehicle location)
(:predicates (at ?v - vehicle ?l - location))

(:functions (maxspeed ?v - vehicle) (speed ?v - vehicle) (traveltime ?v - vehicle) (distance ?p ?l - location))


;; Set the accelerator value over time. Uses time as a 
;; way of avoiding modelling acceleration as a numeric
;; parameter.

(:durative-action accelerate
:parameters (?v - vehicle)
:duration (and (<= ?duration (- (maxspeed ?v) (speed ?v))))
:condition (and )
:effect (and (at end (increase (speed ?v) ?duration)))
)

;; Then drive with the accelerator setting.

(:action drive
:parameters (?v - vehicle ?p ?l - location)
:precondition (and (at ?v ?p) (> (speed ?v) 0))
:effect (and (at ?v ?l) (not (at ?v ?p))
(increase (traveltime ?v) (/ (distance ?p ?l) (speed ?v))))
)


)
