<?php

namespace pr\RecrutementBundle\Entity;

/**
 * Offre
 */
class Offre
{
    /**
     * @var int
     */
    private $id;

    /**
     * @var string
     */
    private $offreTitre;

    /**
     * @var string
     */
    private $offreDescription;

    /**
     * @var string
     */
    private $offreExigence;

    /**
     * @var string
     */
    private $offreSecteur;

    /**
     * @var string
     */
    private $offreLieu;

    /**
     * @var \DateTime
     */
    private $offreDateouverture;

    /**
     * @var \DateTime
     */
    private $offreDatefermeture;

    /**
     * @var int
     */
    private $offreGestionnaireid;

    /**
     * @var int
     */
    private $offreDescriptionid;

    /**
     * @var int
     */
    private $offreExigenceid;


    /**
     * Get id
     *
     * @return int
     */
    public function getId()
    {
        return $this->id;
    }

    /**
     * Set offreTitre
     *
     * @param string $offreTitre
     *
     * @return Offre
     */
    public function setOffreTitre($offreTitre)
    {
        $this->offreTitre = $offreTitre;

        return $this;
    }

    /**
     * Get offreTitre
     *
     * @return string
     */
    public function getOffreTitre()
    {
        return $this->offreTitre;
    }

    /**
     * Set offreDescription
     *
     * @param string $offreDescription
     *
     * @return Offre
     */
    public function setOffreDescription($offreDescription)
    {
        $this->offreDescription = $offreDescription;

        return $this;
    }

    /**
     * Get offreDescription
     *
     * @return string
     */
    public function getOffreDescription()
    {
        return $this->offreDescription;
    }

    /**
     * Set offreExigence
     *
     * @param string $offreExigence
     *
     * @return Offre
     */
    public function setOffreExigence($offreExigence)
    {
        $this->offreExigence = $offreExigence;

        return $this;
    }

    /**
     * Get offreExigence
     *
     * @return string
     */
    public function getOffreExigence()
    {
        return $this->offreExigence;
    }

    /**
     * Set offreSecteur
     *
     * @param string $offreSecteur
     *
     * @return Offre
     */
    public function setOffreSecteur($offreSecteur)
    {
        $this->offreSecteur = $offreSecteur;

        return $this;
    }

    /**
     * Get offreSecteur
     *
     * @return string
     */
    public function getOffreSecteur()
    {
        return $this->offreSecteur;
    }

    /**
     * Set offreLieu
     *
     * @param string $offreLieu
     *
     * @return Offre
     */
    public function setOffreLieu($offreLieu)
    {
        $this->offreLieu = $offreLieu;

        return $this;
    }

    /**
     * Get offreLieu
     *
     * @return string
     */
    public function getOffreLieu()
    {
        return $this->offreLieu;
    }

    /**
     * Set offreDateouverture
     *
     * @param \DateTime $offreDateouverture
     *
     * @return Offre
     */
    public function setOffreDateouverture($offreDateouverture)
    {
        $this->offreDateouverture = $offreDateouverture;

        return $this;
    }

    /**
     * Get offreDateouverture
     *
     * @return \DateTime
     */
    public function getOffreDateouverture()
    {
        return $this->offreDateouverture;
    }

    /**
     * Set offreDatefermeture
     *
     * @param \DateTime $offreDatefermeture
     *
     * @return Offre
     */
    public function setOffreDatefermeture($offreDatefermeture)
    {
        $this->offreDatefermeture = $offreDatefermeture;

        return $this;
    }

    /**
     * Get offreDatefermeture
     *
     * @return \DateTime
     */
    public function getOffreDatefermeture()
    {
        return $this->offreDatefermeture;
    }

    /**
     * Set offreGestionnaireid
     *
     * @param integer $offreGestionnaireid
     *
     * @return Offre
     */
    public function setOffreGestionnaireid($offreGestionnaireid)
    {
        $this->offreGestionnaireid = $offreGestionnaireid;

        return $this;
    }

    /**
     * Get offreGestionnaireid
     *
     * @return int
     */
    public function getOffreGestionnaireid()
    {
        return $this->offreGestionnaireid;
    }

    /**
     * Set offreDescriptionid
     *
     * @param integer $offreDescriptionid
     *
     * @return Offre
     */
    public function setOffreDescriptionid($offreDescriptionid)
    {
        $this->offreDescriptionid = $offreDescriptionid;

        return $this;
    }

    /**
     * Get offreDescriptionid
     *
     * @return int
     */
    public function getOffreDescriptionid()
    {
        return $this->offreDescriptionid;
    }

    /**
     * Set offreExigenceid
     *
     * @param integer $offreExigenceid
     *
     * @return Offre
     */
    public function setOffreExigenceid($offreExigenceid)
    {
        $this->offreExigenceid = $offreExigenceid;

        return $this;
    }

    /**
     * Get offreExigenceid
     *
     * @return int
     */
    public function getOffreExigenceid()
    {
        return $this->offreExigenceid;
    }
}
