<?php

namespace pr\StatBundle\Entity;

/**
 * salarie
 */
class salarie
{
    /**
     * @var int
     */
    private $id;

    /**
     * @var string
     */
    private $nom;

    /**
     * @var string
     */
    private $prenom;

    /**
     * @var \DateTime
     */
    private $dateNaissance;

    /**
     * @var string
     */
    private $sexe;

    /**
     * @var \DateTime
     */
    private $dateEntre;

    /**
     * @var string
     */
    private $typeContrat;

    /**
     * @var int
     */
    private $dureeContrat;

    /**
     * @var int
     */
    private $salaire;

    /**
     * @var int
     */
    private $superieurHierarchique;


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
     * Set nom
     *
     * @param string $nom
     *
     * @return salarie
     */
    public function setNom($nom)
    {
        $this->nom = $nom;

        return $this;
    }

    /**
     * Get nom
     *
     * @return string
     */
    public function getNom()
    {
        return $this->nom;
    }

    /**
     * Set prenom
     *
     * @param string $prenom
     *
     * @return salarie
     */
    public function setPrenom($prenom)
    {
        $this->prenom = $prenom;

        return $this;
    }

    /**
     * Get prenom
     *
     * @return string
     */
    public function getPrenom()
    {
        return $this->prenom;
    }

    /**
     * Set dateNaissance
     *
     * @param \DateTime $dateNaissance
     *
     * @return salarie
     */
    public function setDateNaissance($dateNaissance)
    {
        $this->dateNaissance = $dateNaissance;

        return $this;
    }

    /**
     * Get dateNaissance
     *
     * @return \DateTime
     */
    public function getDateNaissance()
    {
        return $this->dateNaissance;
    }

    /**
     * Set sexe
     *
     * @param string $sexe
     *
     * @return salarie
     */
    public function setSexe($sexe)
    {
        $this->sexe = $sexe;

        return $this;
    }

    /**
     * Get sexe
     *
     * @return string
     */
    public function getSexe()
    {
        return $this->sexe;
    }

    /**
     * Set dateEntre
     *
     * @param \DateTime $dateEntre
     *
     * @return salarie
     */
    public function setDateEntre($dateEntre)
    {
        $this->dateEntre = $dateEntre;

        return $this;
    }

    /**
     * Get dateEntre
     *
     * @return \DateTime
     */
    public function getDateEntre()
    {
        return $this->dateEntre;
    }

    /**
     * Set typeContrat
     *
     * @param string $typeContrat
     *
     * @return salarie
     */
    public function setTypeContrat($typeContrat)
    {
        $this->typeContrat = $typeContrat;

        return $this;
    }

    /**
     * Get typeContrat
     *
     * @return string
     */
    public function getTypeContrat()
    {
        return $this->typeContrat;
    }

    /**
     * Set dureeContrat
     *
     * @param integer $dureeContrat
     *
     * @return salarie
     */
    public function setDureeContrat($dureeContrat)
    {
        $this->dureeContrat = $dureeContrat;

        return $this;
    }

    /**
     * Get dureeContrat
     *
     * @return int
     */
    public function getDureeContrat()
    {
        return $this->dureeContrat;
    }

    /**
     * Set salaire
     *
     * @param integer $salaire
     *
     * @return salarie
     */
    public function setSalaire($salaire)
    {
        $this->salaire = $salaire;

        return $this;
    }

    /**
     * Get salaire
     *
     * @return int
     */
    public function getSalaire()
    {
        return $this->salaire;
    }

    /**
     * Set superieurHierarchique
     *
     * @param integer $superieurHierarchique
     *
     * @return salarie
     */
    public function setSuperieurHierarchique($superieurHierarchique)
    {
        $this->superieurHierarchique = $superieurHierarchique;

        return $this;
    }

    /**
     * Get superieurHierarchique
     *
     * @return int
     */
    public function getSuperieurHierarchique()
    {
        return $this->superieurHierarchique;
    }
}

