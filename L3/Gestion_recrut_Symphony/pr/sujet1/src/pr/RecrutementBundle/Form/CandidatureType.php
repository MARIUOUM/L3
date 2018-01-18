<?php

namespace pr\RecrutementBundle\Form;

use Symfony\Component\Form\AbstractType;
use Symfony\Component\Form\FormBuilderInterface;
use Symfony\Component\OptionsResolver\OptionsResolver;

class CandidatureType extends AbstractType
{
    /**
     * @param FormBuilderInterface $builder
     * @param array $options
     */
    public function buildForm(FormBuilderInterface $builder, array $options)
    {
        $builder
            ->add('candOffreid')
            ->add('candNom')
            ->add('candPrenom')
            ->add('candDatenaissance', 'date')
            ->add('candDiplome')
            ->add('candUniversite')
            ->add('candAnneObtentionDiplome')
            ->add('candExperiencemois')
            ->add('candLettremotivation')
            ->add('candDatedeposition', 'datetime')
            ->add('candLiencv')
            ->add('candEtat')
        ;
    }
    
    /**
     * @param OptionsResolver $resolver
     */
    public function configureOptions(OptionsResolver $resolver)
    {
        $resolver->setDefaults(array(
            'data_class' => 'pr\RecrutementBundle\Entity\Candidature'
        ));
    }
}
