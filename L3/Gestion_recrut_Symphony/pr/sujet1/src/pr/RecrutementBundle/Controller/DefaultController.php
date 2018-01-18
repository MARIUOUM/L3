<?php

namespace pr\RecrutementBundle\Controller;

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



        if (isset($_POST['rechercherbtn']))
        {

                $titreoffre = $_POST['recherche'];
            $repository = $this -> getDoctrine()->getRepository("prRecrutementBundle:Offre");
            $qb = $repository-> createQueryBuilder('o');
            $resultat= $qb
            -> select('o.id'
                ,'o.offreTitre'
                ,'o.offreDescription'
                ,'o.offreExigence'
                ,'o.offreSecteur'
                ,'o.offreLieu'
                ,'o.offreDateouverture'
                ,'o.offreDatefermeture'
                ,'o.offreGestionnaireid'
                ,'o.offreDescriptionid'
                ,'o.offreExigenceid')
            ->where( $qb->expr()->like('o.offreTitre', $qb->expr()->literal('%' . $titreoffre . '%')) )
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

        return $this->render('prRecrutementBundle:Default:index.html.twig',array(
            'offres'=>$ab , 'users'=> $user));
        }

        else {

    	$em = $this->getdoctrine()->getEntityManager();
    	$ab= $em->getRepository("prRecrutementBundle:Offre")->findAll();
$user='';
        if( $this->container->get( 'security.authorization_checker' )->isGranted( 'IS_AUTHENTICATED_FULLY' ) )
{
    $user = $this->container->get('security.token_storage')->getToken()->getUser();
    $userid = $user->getid();

    $em = $this->getdoctrine()->getEntityManager();
        $user= $em->getRepository("AppBundle:user")->findById($userid);
}


        return $this->render('prRecrutementBundle:Default:index.html.twig',array(
        	'offres'=>$ab , 'users'=> $user));
    }

    }

        public function offreAction()
    {

    	 //Je récupère la requete

         if (! isset($_POST['submit1']))
         {
       $id = $_POST['idoffre']; //////Comme PHP

       	$em = $this->getdoctrine()->getEntityManager();
    	$ab= $em->getRepository("prRecrutementBundle:Offre")->findById($id);

$user='';
        if( $this->container->get( 'security.authorization_checker' )->isGranted( 'IS_AUTHENTICATED_FULLY' ) )
{
    $user = $this->container->get('security.token_storage')->getToken()->getUser();
    $userid = $user->getid();

    $em = $this->getdoctrine()->getEntityManager();
        $user= $em->getRepository("AppBundle:user")->findById($userid);
}

    	return $this->render('prRecrutementBundle:Default:offre.html.twig', array('offres'=>$ab, 'users'=> $user));
        }
        else {
                # code...
           $em = $this->getdoctrine()->getEntityManager();
        $ab= $em->getRepository("prRecrutementBundle:Offre")->findAll();

        $user='';
        if( $this->container->get( 'security.authorization_checker' )->isGranted( 'IS_AUTHENTICATED_FULLY' ) )
{
    $user = $this->container->get('security.token_storage')->getToken()->getUser();
    $userid = $user->getid();

    $em = $this->getdoctrine()->getEntityManager();
        $user= $em->getRepository("AppBundle:user")->findById($userid);
}


        return $this->render('prRecrutementBundle:Default:index.html.twig',array(
            'offres'=>$ab, 'users'=> $user));
            }

    }


       public function DeposerAction()
    {
         if (isset($_POST['go']))
    {

        $id = $_POST['idoffre']; //////Comme PHP
    //$Candidature = new Candidature();
    $em = $this->getdoctrine()->getEntityManager();
   //$form = $this->createForm(new CandidatureType());
   $ab= $em->getRepository("prRecrutementBundle:Offre")->findById($id);

$user='';
        if( $this->container->get( 'security.authorization_checker' )->isGranted( 'IS_AUTHENTICATED_FULLY' ) )
{
    $user = $this->container->get('security.token_storage')->getToken()->getUser();
    $userid = $user->getid();

    $em = $this->getdoctrine()->getEntityManager();
        $user= $em->getRepository("AppBundle:user")->findById($userid);
}


    	  return $this->render('prRecrutementBundle:Default:deposer.html.twig', array('offres'=>$ab,'users'=> $user));
        }
        elseif (isset($_POST['btn_ajouter']))
        {

                    $ida = $_POST['idoffre2'];
                    $nom=$_POST['nom'];
                    $prenom=$_POST['prenom'];
                    $diplome=$_POST['diplome'];
                    $datenaissance=$_POST['datenaissance'];
                    $universite=$_POST['universite'];
                    $anneediplome=$_POST['anneediplome'];
                    $experience=$_POST['experience'];
                    $lettre=$_POST['lettre'];
                    //$fichier=$_POST['fichier'];
                    $date= new \DateTime();
                   // $datenaissance= date_format();
                    $emm = $this->getdoctrine()->getEntityManager();
                    $c = new Candidature();
$file_name = $_FILES['fichier']['name'];
$pos = strpos($file_name, '.');
 $extention=   substr($file_name,$pos + 1, strlen($file_name)) ;
                 //$extention->getFile()->guessExtension();


                 $uploadDir = $this->container->getParameter('upload_dir');
                 move_uploaded_file(
                 $_FILES['fichier']['tmp_name'],
                 $uploadDir . '/'.$nom.' '.$prenom.'.'.$extention.''

                 );

               $lien= $nom.' '.$prenom.'.'.$extention.'';



                     $c   -> setCandOffreid($ida)
                             -> setCandNom($nom)
                             -> setCandPrenom($prenom)
                             -> setCandDatenaissance(new \DateTime($_POST['datenaissance']))
                             -> setCandDiplome($diplome)
                             -> setCandUniversite($universite)
                             -> setCandAnneObtentionDiplome($anneediplome)
                             -> setCandExperiencemois($experience)
                             -> setCandLettremotivation($lettre)
                            -> setCandEtat('en attente')
                             -> setCandDatedeposition($date)
                             -> setCandLiencv($lien);

                             $emm->persist($c);
                            $emm->flush();




           return $this->render('prRecrutementBundle:Default:insertoffre.html.twig', array('nom'=>$nom));

        }


    }

    public function uploadAction()
{
    // ...

    $form = $this->createFormBuilder($document)
        ->add('name')
        ->add('file')
        ->getForm();

    // ...
}


 public function InsertAction()
 {

$nom="othman";
return $this->render('prRecrutementBundle:Default:insertoffre.html.twig', array('nom'=>$nom));


 }


  public function adminAction()
 {

  if (isset($_POST['supprimer']))
        {

            $offreTitre = $_POST['offreTitre'];
             $offreid = $_POST['offreid'];


            $em = $this->getdoctrine()->getEntityManager();
            $offre = $em->getRepository('prRecrutementBundle:Offre')->find($offreid);
        $cand = $em->getRepository('prRecrutementBundle:Candidature')->find('CandOffreid',$offreid);

$tab = array("tmp"=> $cand);
$tab = array_filter($tab);
         $em->remove($offre);

         if (!empty ($tab))
         {
   $em->remove($cand);
             }


       $em->flush();








        }

        elseif (isset($_POST['rechercherbtn']))
        {
         $titreoffre = $_POST['offretitre'];

 $repository = $this -> getDoctrine()->getRepository("prRecrutementBundle:Offre");
            $qb = $repository-> createQueryBuilder('o');
            $resultat= $qb
            -> select('o.id'
                ,'o.offreTitre'
                ,'o.offreDescription'
                ,'o.offreExigence'
                ,'o.offreSecteur'
                ,'o.offreLieu'
                ,'o.offreDateouverture'
                ,'o.offreDatefermeture'
                ,'o.offreGestionnaireid'
                ,'o.offreDescriptionid'
                ,'o.offreExigenceid')
            ->where( $qb->expr()->like('o.offreTitre', $qb->expr()->literal('%' . $titreoffre . '%')) )
            -> getQuery();
            $ab=$resultat->getResult();

$user='';
        if( $this->container->get( 'security.authorization_checker' )->isGranted( 'IS_AUTHENTICATED_FULLY' ) )
{
    $user = $this->container->get('security.token_storage')->getToken()->getUser();
    $userid = $user->getid();

    $em = $this->getdoctrine()->getEntityManager();
        $user= $em->getRepository("AppBundle:user")->findById($userid);

        return $this->render('prRecrutementBundle:Default:admin.html.twig',array(
            'offres'=>$ab, 'users'=> $user));
}



        }

        else {

$user='';
        if( $this->container->get( 'security.authorization_checker' )->isGranted( 'IS_AUTHENTICATED_FULLY' ) )
{
    $user = $this->container->get('security.token_storage')->getToken()->getUser();
    $userid = $user->getid();

    $em = $this->getdoctrine()->getEntityManager();
        $user= $em->getRepository("AppBundle:user")->findById($userid);
}




        $em = $this->getdoctrine()->getEntityManager();
        $ab= $em->getRepository("prRecrutementBundle:Offre")->findAll();

        return $this->render('prRecrutementBundle:Default:admin.html.twig',array(
            'offres'=>$ab, 'users'=> $user));
        }
    }


public function ajoutoffreAction()
 {
 $user='';
        if( $this->container->get( 'security.authorization_checker' )->isGranted( 'IS_AUTHENTICATED_FULLY' ) )
{
    $user = $this->container->get('security.token_storage')->getToken()->getUser();
    $userid = $user->getid();

    $em = $this->getdoctrine()->getEntityManager();
        $user= $em->getRepository("AppBundle:user")->findById($userid);
}

$em = $this->getdoctrine()->getEntityManager();
        $ab= $em->getRepository("AppBundle:user")->findAll();
        return $this->render('prRecrutementBundle:Default:ajoutoffre.html.twig',array(
            'offres'=>$ab , 'users'=> $user));

 }

public function confirmeajoutoffreAction()
 {

 if (isset($_POST['ajouteroffre']))
    {
          $titre =$_POST['titre'];
          $description =$_POST['description'];
          $exigence =$_POST['exigence'];
          $secteur =$_POST['secteur'];
          $lieu =$_POST['lieu'];
          $datefermeture =$_POST['datefermeture'];
          $gestionnaire =$_POST['gestionnaire'];

$date= new \DateTime();

 $em = $this->getdoctrine()->getEntityManager();
                    $offre = new Offre();





                     $offre  -> setOffreTitre($titre)
                             -> setOffreDescription($description)
                             -> setOffreExigence($exigence)
                             -> setOffreSecteur($secteur)
                             -> setOffreLieu($lieu)
                             -> setOffreDateouverture($date)
                             -> setOffreDatefermeture((new \DateTime($_POST['datefermeture'])))
                             -> setOffreGestionnaireid($gestionnaire)

                             -> setOffreDescriptionid('0')
                             -> setOffreExigenceid('0')


                            -> setOffreDateouverture($date);
                             $em->persist($offre);
                            $em->flush();






           return $this->render('prRecrutementBundle:Default:confirmeajoutoffre.html.twig',
           array('nom'=>$lieu));
    }
else {

$em = $this->getdoctrine()->getEntityManager();
        $ab= $em->getRepository("prRecrutementBundle:Offre")->findAll();

        return $this->render('prRecrutementBundle:Default:admin.html.twig',array(
            'offres'=>$ab));
       }



}


public function gestionnaireAction()
 {


if( $this->container->get( 'security.authorization_checker' )->isGranted( 'IS_AUTHENTICATED_FULLY' ) )
{
    $user = $this->container->get('security.token_storage')->getToken()->getUser();
    $iduser = $user->getid();
}

$em = $this->getdoctrine()->getEntityManager();
        $ab= $em->getRepository("prRecrutementBundle:Offre")->findByoffreGestionnaireid($iduser);

if (empty($ab))
{

$ch= "Vous n'avez aucune offre pour le gérer..";
// return $this->render('prRecrutementBundle:Default:gestionnaire.html.twig', array('offres' => $ch) );
return new response ($ch);
 }


 $user='';
        if( $this->container->get( 'security.authorization_checker' )->isGranted( 'IS_AUTHENTICATED_FULLY' ) )
{
    $user = $this->container->get('security.token_storage')->getToken()->getUser();
    $userid = $user->getid();

    $em = $this->getdoctrine()->getEntityManager();
        $user= $em->getRepository("AppBundle:user")->findById($userid);
}

        return $this->render('prRecrutementBundle:Default:gestionnaire.html.twig',
            array('offres' => $ab , 'users'=>$user )

            );


 }


public function gerercandidatureAction()
 {




 if (isset($_POST['miseajour']))
    {

 $idcandidature =$_POST['idcandidature'];
  $etatcandidature =$_POST['etatcandidature'];




$tmp=$_SESSION['idoffrepublic'];

      $em = $this->getDoctrine()->getManager();
    $candidature = $em->getRepository('prRecrutementBundle:Candidature')->find($idcandidature);



   $candidature->setCandEtat($etatcandidature);
    $em->flush();




    $em = $this->getdoctrine()->getEntityManager();
        $ab= $em->getRepository("prRecrutementBundle:Candidature")->findBycandOffreid($tmp);






$user='';
        if( $this->container->get( 'security.authorization_checker' )->isGranted( 'IS_AUTHENTICATED_FULLY' ) )
{
    $user = $this->container->get('security.token_storage')->getToken()->getUser();
    $userid = $user->getid();

    $em = $this->getdoctrine()->getEntityManager();
        $user= $em->getRepository("AppBundle:user")->findById($userid);
}



   return $this->render('prRecrutementBundle:Default:gerercandidature.html.twig',
            array('offres' => $ab ,'users'=> $user));



    }



 if (isset($_POST['submit1']))
    {


        $idoffrepublic =$_POST['idoffre'];

    $em = $this->getdoctrine()->getEntityManager();
        $ab= $em->getRepository("prRecrutementBundle:Candidature")->findBycandOffreid($idoffrepublic);



$_SESSION['idoffrepublic'] = $idoffrepublic;


$user='';
        if( $this->container->get( 'security.authorization_checker' )->isGranted( 'IS_AUTHENTICATED_FULLY' ) )
{
    $user = $this->container->get('security.token_storage')->getToken()->getUser();
    $userid = $user->getid();

    $em = $this->getdoctrine()->getEntityManager();
        $user= $em->getRepository("AppBundle:user")->findById($userid);
}



   return $this->render('prRecrutementBundle:Default:gerercandidature.html.twig',
            array('offres' => $ab ,'users'=> $user));

        }

        else
        {




$em = $this->getdoctrine()->getEntityManager();
        $ab= $em->getRepository("prRecrutementBundle:Offre")->findAll();
$user='';
        if( $this->container->get( 'security.authorization_checker' )->isGranted( 'IS_AUTHENTICATED_FULLY' ) )
{
    $user = $this->container->get('security.token_storage')->getToken()->getUser();
    $userid = $user->getid();

    $em = $this->getdoctrine()->getEntityManager();
        $user= $em->getRepository("AppBundle:user")->findById($userid);
}


        return $this->render('prRecrutementBundle:Default:index.html.twig',array(
            'offres'=>$ab , 'users'=> $user));








        }



}

public function modifieroffreAction()
 {
 if (isset($_POST['modifier']))
    {


        $offreid =$_POST['offreid'];
         $offregestionnaireid =$_POST['offregestionnaireid'];

         $em = $this->getdoctrine()->getEntityManager();
        $ab= $em->getRepository("prRecrutementBundle:Offre")->findById($offreid);
        $useroffre= $em->getRepository("AppBundle:user")->findById($offregestionnaireid);

$user='';
        if( $this->container->get( 'security.authorization_checker' )->isGranted( 'IS_AUTHENTICATED_FULLY' ) )
{
    $user = $this->container->get('security.token_storage')->getToken()->getUser();
    $userid = $user->getid();

    $em = $this->getdoctrine()->getEntityManager();
        $user= $em->getRepository("AppBundle:user")->findById($userid);
}

         return $this->render('prRecrutementBundle:Default:modifieroffre.html.twig',
            array('offres' => $ab, 'usersoffre' => $useroffre, 'users'=>$user )

            );

 }

}




public function insertmodifieroffreAction()
 {
     if (isset($_POST['modifieroffre']))
    {
         $idoffre =$_POST['idoffre'];
          $titre =$_POST['titre'];
          $description =$_POST['description'];
          $exigence =$_POST['exigence'];
          $secteur =$_POST['secteur'];
          $lieu =$_POST['lieu'];
          $datefermeture =$_POST['datefermeture'];
          $gestionnaire =$_POST['gestionnaire'];



                $em = $this->getDoctrine()->getManager();
    $offre = $em->getRepository('prRecrutementBundle:Offre')->find($idoffre);



    $offre->  setOffreTitre($titre)
          ->  setOffreDescription($description)
          ->  setOffreExigence($exigence)
          ->  setOffreSecteur($secteur)
          ->  setOffreLieu($lieu)
          ->  setOffreDatefermeture(new \DateTime($_POST['datefermeture']))
          ->  setOffreGestionnaireid ($gestionnaire)
            ;
        //$em = persist($offre)   ;
$em->flush();

    return $this->render('prRecrutementBundle:Default:insertmodifieroffre.html.twig');
 }
}



}
