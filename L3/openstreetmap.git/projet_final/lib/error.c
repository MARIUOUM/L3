#include <stdio.h>
#include <stdlib.h>
#include "error.h"

int global_error=0;

char * osmError() {
  	switch(global_error) {
    	default : return "SUCCESS";
  	}
}
