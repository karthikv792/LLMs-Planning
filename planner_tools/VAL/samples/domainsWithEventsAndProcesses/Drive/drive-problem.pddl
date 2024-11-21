(define (problem drive-problem)
(:domain drivedomain)
(:objects  
	car - vehicle
	start - location
	end - location
)
(:init 

	(at car start)
	(= (speed car) 0)
	(= (maxspeed car) 10)
	(= (traveltime car) 0)
	(= (distance start end) 100)
)
(:goal (and (at car end) (>= (traveltime car) 20)))
(:metric minimize (total-time))) 
