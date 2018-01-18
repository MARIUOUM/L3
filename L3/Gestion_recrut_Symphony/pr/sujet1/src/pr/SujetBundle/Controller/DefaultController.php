<?php

namespace pr\SujetBundle\Controller;

use Symfony\Bundle\FrameworkBundle\Controller\Controller;

class DefaultController extends Controller
{
    public function indexAction()
    {
        return $this->render('SBundle:Default:index.html.twig');
    }
}
