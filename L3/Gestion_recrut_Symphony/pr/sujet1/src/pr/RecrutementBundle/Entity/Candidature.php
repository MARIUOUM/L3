<?php

namespace pr\RecrutementBundle\Entity;

/**
 * Candidature
 */
class Candidature
{
    /**
     * @var int
     */
    private $id;

    /**
     * @var int
     */
    private $candOffreid;

    /**
     * @var string
     */
    private $candNom;

    /**
     * @var string
     */
    private $candPrenom;

    /**
     * @var \DateTime
     */
    private $candDatenaissance;

    /**
     * @var string
     */
    private $candDiplome;

    /**
     * @var string
     */
    private $candUniversite;

    /**
     * @var int
     */
    private $candAnneObtentionDiplome;

    /**
     * @var int
     */
    private $candExperiencemois;

    /**
     * @var string
     */
    private $candLettremotivation;

    /**
     * @var \DateTime
     */
    private $candDatedeposition;

    /**
     * @var string
     */
    private $candLiencv;

    /**
     * @var string
     */
    private $candEtat;


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
     * Set candOffreid
     *
     * @param integer $candOffreid
     *
     * @return Candidature
     */
    public function setCandOffreid($candOffreid)
    {
        $this->candOffreid = $candOffreid;

        return $this;
    }

    /**
     * Get candOffreid
     *
     * @return int
     */
    public function getCandOffreid()
    {
        return $this->candOffreid;
    }

    /**
     * Set candNom
     *
     * @param string $candNom
     *
     * @return Candidature
     */
    public function setCandNom($candNom)
    {
        $this->candNom = $candNom;

        return $this;
    }

    /**
     * Get candNom
     *
     * @return string
     */
    public function getCandNom()
    {
        return $this->candNom;
    }

    /**
     * Set candPrenom
     *
     * @param string $candPrenom
     *
     * @return Candidature
     */
    public function setCandPrenom($candPrenom)
    {
        $this->candPrenom = $candPrenom;

        return $this;
    }

    /**
     * Get candPrenom
     *
     * @return string
     */
    public function getCandPrenom()
    {
        return $this->candPrenom;
    }

    /**
     * Set candDatenaissance
     *
     * @param \DateTime $candDatenaissance
     *
     * @return Candidature
     */
    public function setCandDatenaissance($candDatenaissance)
    {
        $this->candDatenaissance = $candDatenaissance;

        return $this;
    }

    /**
     * Get candDatenaissance
     *
     * @return \DateTime
     */
    public function getCandDatenaissance()
    {
        return $this->candDatenaissance;
    }

    /**
     * Set candDiplome
     *
     * @param string $candDiplome
     *
     * @return Candidature
     */
    public function setCandDiplome($candDiplome)
    {
        $this->candDiplome = $candDiplome;

        return $this;
    }

    /**
     * Get candDiplome
     *
     * @return string
     */
    public function getCandDiplome()
    {
        return $this->candDiplome;
    }

    /**
     * Set candUniversite
     *
     * @param string $candUniversite
     *
     * @return Candidature
     */
    public function setCandUniversite($candUniversite)
    {
        $this->candUniversite = $candUniversite;

        return $this;
    }

    /**
     * Get candUniversite
     *
     * @return string
     */
    public function getCandUniversite()
    {
        return $this->candUniversite;
    }

    /**
     * Set candAnneObtentionDiplome
     *
     * @param integer $candAnneObtentionDiplome
     *
     * @return Candidature
     */
    public function setCandAnneObtentionDiplome($candAnneObtentionDiplome)
    {
        $this->candAnneObtentionDiplome = $candAnneObtentionDiplome;

        return $this;
    }

    /**
     * Get candAnneObtentionDiplome
     *
     * @return int
     */
    public function getCandAnneObtentionDiplome()
    {
        return $this->candAnneObtentionDiplome;
    }

    /**
     * Set candExperiencemois
     *
     * @param integer $candExperiencemois
     *
     * @return Candidature
     */
    public function setCandExperiencemois($candExperiencemois)
    {
        $this->candExperiencemois = $candExperiencemois;

        return $this;
    }

    /**
     * Get candExperiencemois
     *
     * @return int
     */
    public function getCandExperiencemois()
    {
        return $this->candExperiencemois;
    }

    /**
     * Set candLettremotivation
     *
     * @param string $candLettremotivation
     *
     * @return Candidature
     */
    public function setCandLettremotivation($candLettremotivation)
    {
        $this->candLettremotivation = $candLettremotivation;

        return $this;
    }

    /**
     * Get candLettremotivation
     *
     * @return string
     */
    public function getCandLettremotivation()
    {
        return $this->candLettremotivation;
    }

    /**
     * Set candDatedeposition
     *
     * @param \DateTime $candDatedeposition
     *
     * @return Candidature
     */
    public function setCandDatedeposition($candDatedeposition)
    {
        $this->candDatedeposition = $candDatedeposition;

        return $this;
    }

    /**
     * Get candDatedeposition
     *
     * @return \DateTime
     */
    public function getCandDatedeposition()
    {
        return $this->candDatedeposition;
    }

    /**
     * Set candLiencv
     *
     * @param string $candLiencv
     *
     * @return Candidature
     */
    public function setCandLiencv($candLiencv)
    {
        $this->candLiencv = $candLiencv;

        return $this;
    }

    /**
     * Get candLiencv
     *
     * @return string
     */
    public function getCandLiencv()
    {
        return $this->candLiencv;
    }

    /**
     * Set candEtat
     *
     * @param string $candEtat
     *
     * @return Candidature
     */
    public function setCandEtat($candEtat)
    {
        $this->candEtat = $candEtat;

        return $this;
    }

    /**
     * Get candEtat
     *
     * @return string
     */
    public function getCandEtat()
    {
        return $this->candEtat;
    }
}

