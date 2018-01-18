<?php

namespace pr\BookBundle\Entity;
use Doctrine\ORM\Mapping as ORM;


/**
 * contact
 *
 * @ORM\Table(name="contact")
 * @ORM\Entity
 */
class contact
{
  /**
     *
     * @ORM\Column(type="integer")
     * @ORM\Id
     * @ORM\GeneratedValue(strategy="AUTO")
     */
    private $id;

    /**
       *
       * @ORM\Column(type="string")
       * @ORM\GeneratedValue(strategy="AUTO")
       */
    private $nom;

    /**
       *
       * @ORM\Column(type="string")
       * @ORM\GeneratedValue(strategy="AUTO")
       */
    private $prenom;

    /**
       *
       * @ORM\Column(type="string")
       * @ORM\GeneratedValue(strategy="AUTO")
       */

    private $mail;

    /**
       *
       * @ORM\Column(type="string")
       * @ORM\GeneratedValue(strategy="AUTO")
       */

    private $telephone;

    /**
       *
       * @ORM\Column(type="string")
       * @ORM\GeneratedValue(strategy="AUTO")
       */

    private $adresse;

    /**
     * Get id
     *
     * @return int
     */
    public function getId()
    {
        return $this->id;
    }

    public function getNom()
    {
        return $this->nom;
    }

    public function getPrenom()
    {
        return $this->prenom;
    }


    public function getMail()
    {
        return $this->mail;
    }

    public function getTelephone()
    {
        return $this->telephone;
    }

    public function getAdresse()
    {
        return $this->adresse;
    }


}
