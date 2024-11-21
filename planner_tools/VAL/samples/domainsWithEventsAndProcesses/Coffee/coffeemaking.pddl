(define (domain COFFEE)
(:requirements :typing :durative-actions :fluents :timed-initial-literals :time
		:duration-inequalities)

(:types coffee water)

(:predicates 
	(havecoffee ?c - coffee)
	(hot ?w - water)
	(cold ?w - water)
	(madecoffee ?c - coffee ?w - water)
	(heating ?w - water)
	(cooling ?w - water)
	(boiled ?w - water)
)

(:functions 
	(temperature ?w - water)

)

(:durative-action makecoffee
:parameters (?c - coffee ?w - water)
:duration (>= ?duration 1)
:condition (and 
		(at start (boiled ?w))
		(over all (>= (temperature ?w) 60))
		(over all (<= (temperature ?w) 80))
		 (at start (havecoffee ?c))		
          	)
:effect (and (at end (madecoffee ?c ?w))
		)
)

(:action heatwater
:parameters (?w - water)
:precondition (and (cold ?w))
:effect (and (not (cold ?w))
		(heating ?w)
		)
)

(:process heating
:parameters (?w - water)
:precondition (and (heating ?w) (<= (temperature ?w) 100))
:effect (increase (temperature ?w) (* #t 2))
)

(:event boil
:parameters (?w - water)
:precondition (and (>= (temperature ?w) 100) 
			(not (boiled ?w)))
:effect (and (boiled ?w))
)

(:event stop-heating
:parameters (?w - water)
:precondition (and (>= (temperature ?w) 100) (heating ?w))
:effect (and (not (heating ?w)))
)

(:process cooling
:parameters (?w - water)
:precondition (>= (temperature ?w) 18)
:effect (decrease (temperature ?w) (* #t 0.5))
)

(:event stop-cooling
:parameters (?w - water)
:precondition (and (<= (temperature ?w) 18) (cooling ?w))
	;; Assume we are working with a room temperature of
	;; 18 degrees
:effect (and (not (cooling ?w)))
)




)