<?php

namespace pr\StatBundle\Controller;

use Symfony\Bundle\FrameworkBundle\Controller\Controller;
use pr\StatBundle\Entity\salarie;

class DefaultController extends Controller
{
    public function indexAction()
    {

    	$em = $this->getdoctrine()->getEntityManager();
    	//$ab= $em->getRepository("StatBundle:salarie")->findAll();
       //   $repository = $this -> getDoctrine()->getRepository("StatBundle:salarie");
           // $qb = $repository-> createQueryBuilder('s');
            //$resultat= $qb
           // -> select('s.id'
                //,'s.nom'
               // ,'s.prenom'
               // ,'s.dateNaissance'
               // ,'s.sexe'
              //  ,'s.dateEntre'
               // ,'s.typeContrat'
               // ,'s.dureeContrat'
               // ,'s.salaire'
               // ,'s.superieurHierarchique'
               
           // ->where( $qb->expr()->like('o.offreTitre', $qb->expr()->literal('%' . $titreoffre . '%')) )
           // -> getQuery();
            //$ab=$resultat->getResult();


    	$qb = $em->createQueryBuilder();
$qb->select('count(salarie.id)');
$qb->from('StatBundle:salarie','salarie')
->where( $qb->expr()->like('salarie.sexe', $qb->expr()->literal('F')));

$countfemme = $qb->getQuery()->getSingleScalarResult();



    	$qb = $em->createQueryBuilder();
$qb->select('count(salarie.id)');
$qb->from('StatBundle:salarie','salarie')
->where( $qb->expr()->like('salarie.sexe', $qb->expr()->literal('M')));

$counthomme = $qb->getQuery()->getSingleScalarResult();

    	$qb = $em->createQueryBuilder();
$qb->select('count(salarie.id)');
$qb->from('StatBundle:salarie','salarie')
->where('salarie.salaire < 2000');
$countunder2= $qb->getQuery()->getSingleScalarResult();


    	$qb = $em->createQueryBuilder();
$qb->select('count(salarie.id)');
$qb->from('StatBundle:salarie','salarie')
->where('salarie.salaire >= 2000 and salarie.salaire <=3999');
$countsalaire2_4= $qb->getQuery()->getSingleScalarResult();

    	$qb = $em->createQueryBuilder();
$qb->select('count(salarie.id)');
$qb->from('StatBundle:salarie','salarie')
->where('salarie.salaire >= 4000 and salarie.salaire <=5999');
$countsalaire4_6= $qb->getQuery()->getSingleScalarResult();



    	$qb = $em->createQueryBuilder();
$qb->select('count(salarie.id)');
$qb->from('StatBundle:salarie','salarie')
->where('salarie.salaire >=6000');
$countover6= $qb->getQuery()->getSingleScalarResult();


    	$qb = $em->createQueryBuilder();
$qb->select('count(salarie.id)');
$qb->from('StatBundle:salarie','salarie')
->where( $qb->expr()->like('salarie.typeContrat', $qb->expr()->literal('CDD')));

$countcdd = $qb->getQuery()->getSingleScalarResult();


    	$qb = $em->createQueryBuilder();
$qb->select('count(salarie.id)');
$qb->from('StatBundle:salarie','salarie')
->where( $qb->expr()->like('salarie.typeContrat', $qb->expr()->literal('CDI')));

$countcdi = $qb->getQuery()->getSingleScalarResult();


    	$qb = $em->createQueryBuilder();
$qb->select('count(salarie.id)');
$qb->from('StatBundle:salarie','salarie')
->where( $qb->expr()->like('salarie.typeContrat', $qb->expr()->literal('sta')));

$countsta = $qb->getQuery()->getSingleScalarResult();




    	$qb = $em->createQueryBuilder();
$qb->select('count(salarie.id)');
$qb->from('StatBundle:salarie','salarie')
->where( $qb->expr()->like('salarie.typeContrat', $qb->expr()->literal('vol')));

$countvol = $qb->getQuery()->getSingleScalarResult();


        return $this->render('StatBundle:Default:index.html.twig',
        	array('salaries'=>array(

        		'salariesf'=>$countfemme,
        		'salariesh'=>$counthomme,
        		'salarieunder2'=>$countunder2,
        		'countsalaire2_4'=>$countsalaire2_4,
        		'countsalaire4_6'=>$countsalaire4_6,
        		'countover6'=>$countover6 ,
        		'countcdd'=>$countcdd,
        		'countcdi'=>$countcdi,
        		'countsta'=>$countsta,
        		'countvol'=>$countvol



        		))
          
        	




        	);
    }
}
