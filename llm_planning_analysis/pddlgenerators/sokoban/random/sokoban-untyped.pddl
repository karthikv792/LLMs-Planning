(define (domain sokoban-untyped)
(:predicates 
	     (location ?x)
             (direction ?d)
             (box ?b)
             (at-robot ?l)
             (at ?o ?l)
             (adjacent ?l1 ?l2 ?d) 
             (clear ?l)
)


(:action move
:parameters (?from ?to ?dir)
:precondition (and (location ?from) (location ?to) (clear ?to) (direction ?dir)
                   (at-robot ?from) (adjacent ?from ?to ?dir))
:effect (and (at-robot ?to) (not (at-robot ?from)))
)
             

(:action push
:parameters  (?rloc ?bloc ?floc ?dir ?b)
:precondition (and (location ?rloc) (location ?floc) (location ?bloc)
		   (at-robot ?rloc) (box ?b) (at ?b ?bloc) (clear ?floc)
	           (direction ?dir) (adjacent ?rloc ?bloc ?dir) (adjacent ?bloc ?floc ?dir))

:effect (and (at-robot ?bloc) (at ?b ?floc) (clear ?bloc)
             (not (at-robot ?rloc)) (not (at ?b ?bloc)) (not (clear ?floc)))
)

)


