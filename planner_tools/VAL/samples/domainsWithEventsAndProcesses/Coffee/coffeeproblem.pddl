(define (problem COFFEE1)
(:domain COFFEE)
(:objects 
	water1 - water
	coffee1 - coffee
)

(:init 
	(havecoffee coffee1)
	(cold water1)
	(= (temperature water1) 7) ;; Cold tap water at 7 degrees 
)

(:goal (and (madecoffee coffee1 water1)))

)