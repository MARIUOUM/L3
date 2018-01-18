<?php

namespace pr\BookBundle\Controller;


use pr\BookBundle\Entity\contact;

use Symfony\Bundle\FrameworkBundle\Controller\Controller;
use Symfony\Component\Routing\RouteCollection;
use Symfony\Component\Routing\Route;
use Symfony\Component\HttpFoundation\Response;
use Symfony\Component\HttpFoundation\Request;
use pr\RecrutementBundle\Entity\Candidature;
use pr\RecrutementBundle\Entity\Offre;
use pr\RecrutementBundle\Form\CandidatureType;
use pr\AppBundle\Entity\User;
use Symfony\Component\HttpFoundation\File\UploadedFile;
use Symfony\Component\DependencyInjection\ContainerBuilder;
use Symfony\Component\Yaml\Dumper;
use Doctrine\Bundle\DoctrineBundle\Registry;



class DefaultController extends Controller
{
    public function indexAction()
    {
      // $em = $this->getdoctrine()->getEntityManager();
      // $ab= $em->getRepository("prBookBundle:contact")->findAll();
      // return $this->render('prBookBundle:Default:index.html.twig', array('salarie' => $ab));


    if (isset($_POST['rechercherbtn']))
    {

            $titreoffre = $_POST['recherche'];
        $repository = $this -> getDoctrine()->getRepository("prBookBundle:contact");
        $qb = $repository-> createQueryBuilder('o');
        $resultat= $qb
        -> select('o.id'
            ,'o.nom'
            ,'o.prenom'
            ,'o.mail'
            ,'o.telephone'
            ,'o.adresse')
        ->where( $qb->expr()->like('o.nom', $qb->expr()->literal('%' . $titreoffre . '%')) )
        -> getQuery();
        $ab=$resultat->getResult();


       // $titreoffre = $_POST['recherche'];
       // $em = $this->getdoctrine()->getEntityManager();



   // $ab= $em->getRepository("prRecrutementBundle:Offre")->findByoffreTitre($titreoffre);

$user='';

if( $this->container->get( 'security.authorization_checker' )->isGranted( 'IS_AUTHENTICATED_FULLY' ) )
{
$user = $this->container->get('security.token_storage')->getToken()->getUser();
$userid = $user->getid();

$em = $this->getdoctrine()->getEntityManager();
    $user= $em->getRepository("AppBundle:user")->findById($userid);
}

// get the Query from the QueryBuilder here ...

    return $this->render('prBookBundle:Default:index.html.twig',array(
        'contact'=>$ab , 'users'=> $user));
    }

    else {

  $em = $this->getdoctrine()->getEntityManager();
  $ab= $em->getRepository("prBookBundle:contact")->findAll();
$user='';
    if( $this->container->get( 'security.authorization_checker' )->isGranted( 'IS_AUTHENTICATED_FULLY' ) )
{
$user = $this->container->get('security.token_storage')->getToken()->getUser();
$userid = $user->getid();

$em = $this->getdoctrine()->getEntityManager();
    $user= $em->getRepository("AppBundle:user")->findById($userid);
}


    return $this->render('prBookBundle:Default:index.html.twig',array(
      'contact'=>$ab , 'users'=> $user));
}

    }

    public function contactAction()
  {

    //Je récupère la requete

      if (! isset($_POST['submit1']))
      {
        $id = $_POST['idcontact']; //////Comme PHP

         $em = $this->getdoctrine()->getEntityManager();
       $ab= $em->getRepository("prBookBundle:contact")->findById($id);

 $user='';
     if( $this->container->get( 'security.authorization_checker' )->isGranted( 'IS_AUTHENTICATED_FULLY' ) )
 {
 $user = $this->container->get('security.token_storage')->getToken()->getUser();
 $userid = $user->getid();

 $em = $this->getdoctrine()->getEntityManager();
     $user= $em->getRepository("AppBundle:user")->findById($userid);
 }

   return $this->render('prBookBundle:Default:contact.html.twig', array('contact'=>$ab, 'users'=> $user));
     }
     else {
             # code...
        $em = $this->getdoctrine()->getEntityManager();
     $ab= $em->getRepository("prBookBundle:contact")->findAll();

     $user='';
     if( $this->container->get( 'security.authorization_checker' )->isGranted( 'IS_AUTHENTICATED_FULLY' ) )
 {
 $user = $this->container->get('security.token_storage')->getToken()->getUser();
 $userid = $user->getid();

 $em = $this->getdoctrine()->getEntityManager();
     $user= $em->getRepository("AppBundle:user")->findById($userid);
 }


     return $this->render('prBookBundle:Default:index.html.twig',array(
         'contact'=>$ab, 'users'=> $user));
         }

}
}
