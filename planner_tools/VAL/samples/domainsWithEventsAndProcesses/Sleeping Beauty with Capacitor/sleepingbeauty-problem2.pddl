(define (problem sleepingbeauty-problem)
(:domain sleepingbeauty)
(:objects )
(:init (= (charge) 0)
	(= (resistance) 2) ;; Resistance of 1 is just small enough to
		;; avoid the rousing event, but smaller than this will
		;; cause the alarm to wake her up!
	(= (ringtime) 0)
	(windowclosed)
	(magnetoperational)
	(alarmdisabled)
	(deeplyasleep)
)
(:goal (and (freshair) (deeplyasleep)))
(:metric minimize (total-time))) 
