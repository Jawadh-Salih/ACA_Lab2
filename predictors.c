
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "predictors.h"
#include "framework.h"

/* Random prediction */
void random_predictor() {
    /* Variable to store the prediction you predict for this branch. */
    bool prediction = false;

    /* Variable to store the the address of the branch. */
    uint32_t addr = 0;
    
    /* 
     * Variable to store the actual branch result 
     * (obtained from the predictor library)
     */
    bool actual = false;

    /* 
     * Prediction loop, until tracefile is empty.
     */
    while (predictor_getState() != DONE) {

        /* Get the next branch address from the state machine. */
        if (predictor_getNextBranch(&addr) != 0) {
            fprintf(stderr, "ERROR: \"predictor_getNextBranch()\" called in "\
                   "a state it shouldn't be called!\n");
        }

        /* Do a random prediction */
        prediction = rand() & 1;


        /* 
         * Feed the prediction to the state machine, and get the actual
         * result back.
         */
        if (predictor_predict(prediction, &actual) != 0) {
            fprintf(stderr, "ERROR: \"predictor_predict()\" called in "\
                    " a state it shouldn't be called\n");
        }
        
    }
}

/* Predict always true (taken) or false */
void always_x(bool p) {
    /* Variable to store the prediction you predict for this branch. */
    bool prediction = p;

    /* Variable to store the the address of the branch. */
    uint32_t addr = 0;
    
    /* 
     * Variable to store the actual branch result 
     * (obtained from the predictor library)
     */
    bool actual = false;

    /* 
     * Prediction loop, until tracefile is empty.
     */
    while (predictor_getState() != DONE) {

        /* Get the next branch address from the state machine. */
        if (predictor_getNextBranch(&addr) != 0) {
            fprintf(stderr, "ERROR: \"predictor_getNextBranch()\" called in "\
                   "a state it shouldn't be called!\n");
        }

        /* Do a prediction */
        prediction = p;

        /* 
         * Feed the prediction to the state machine, and get the actual
         * result back.
         */
        if (predictor_predict(prediction, &actual) != 0) {
            fprintf(stderr, "ERROR: \"predictor_predict()\" called in "\
                    " a state it shouldn't be called\n");
        }
        
    }
}
void single_bit_predictor(){
    //printf("%d\n", sizeof(bool));

    bool * BHT = (bool * )malloc(8192*sizeof(bool));
    memset(BHT,false,8192);

    bool prediction = false;

    /* Variable to store the the address of the branch. */
    uint32_t addr = 0;

    /*
     * Variable to store the actual branch result
     * (obtained from the predictor library)
     */
    bool actual = false;

    /*
     * Prediction loop, until tracefile is empty.
     */
    while (predictor_getState() != DONE) {

        /* Get the next branch address from the state machine. */
        if (predictor_getNextBranch(&addr) != 0) {
            fprintf(stderr, "ERROR: \"predictor_getNextBranch()\" called in "\
               "a state it shouldn't be called!\n");
        }

        // making a 13 bit suffix.
        int index = addr & 0b1111111111111;
        /* Do a prediction */
        prediction = BHT[index];


        /*
         * Feed the prediction to the state machine, and get the actual
         * result back.
         */
        if (predictor_predict(prediction, &actual) != 0) {
            fprintf(stderr, "ERROR: \"predictor_predict()\" called in "\
                " a state it shouldn't be called\n");
        }
        // store the actual state of the branch so that it can be used as the next prediction
        BHT[index] = actual;

    }
    free(BHT);


}
void two_bit_predictor() {

    typedef struct two_bit {
        bool b1;
        bool b2;
    }two_Bit;
    two_Bit *BHT = (struct two_bit *) malloc(4096 * sizeof(struct two_bit));

    memset(BHT,false,4096 * sizeof(struct two_bit));

    bool prediction = false;

    /* Variable to store the the address of the branch. */
    uint32_t addr = 0;

    /*
     * Variable to store the actual branch result
     * (obtained from the predictor library)
     */
    bool actual = true;

    /*
     * Prediction loop, until tracefile is empty.
     */
    while (predictor_getState() != DONE) {

        /* Get the next branch address from the state machine. */
        if (predictor_getNextBranch(&addr) != 0) {
            fprintf(stderr, "ERROR: \"predictor_getNextBranch()\" called in "\
                   "a state it shouldn't be called!\n");
        }

        // making a 12 bit suffix.
        int index = addr & 0b111111111111;
        /* Do a prediction as it is in a state machine of 2 bits. */

        // only need to check the MSB to predict as it's
        // only bit changes according to the state machine.
        if (!BHT[index].b1)
            prediction = false; // predict not taken
        else
            prediction = true;// predict taken

        /*
         * Feed the prediction to the state machine, and get the actual
         * result back.
         */
        if (predictor_predict(prediction, &actual) != 0) {
            fprintf(stderr, "ERROR: \"predictor_predict()\" called in "\
                    " a state it shouldn't be called\n");
        }
        // store the actual state of the branch so that it can be used as the next prediction
        // if the branch is taken then change the states of the index.
        if (actual) {// branch is actually taken.
            // if 00 goto 01
            if (!BHT[index].b1 && !BHT[index].b2) // 00
                BHT[index].b1 = false, BHT[index].b2 = actual;// 01
                // else ( 01 10 11) goto 11
            else if ((!BHT[index].b1 && BHT[index].b2) || (BHT[index].b1 && !BHT[index].b2)
                     || (BHT[index].b1 && BHT[index].b2)) // 11
                BHT[index].b1 = true, BHT[index].b2 = actual;// 11
        }
        else { // branch is not taken
            if (BHT[index].b1 && BHT[index].b2) // 11
                BHT[index].b1 = true, BHT[index].b2 = actual; // 10
                // if 00 01 10 is there goto state 00
            else if ((!BHT[index].b1 && !BHT[index].b2) || (!BHT[index].b1 && BHT[index].b2) || (BHT[index].b1 && !BHT[index].b2)) // 11
                BHT[index].b1 = false, BHT[index].b2 = actual; // 00
        }


    }
    free(BHT);

}

void two_two_bit_predictor(){
    // Create the data structure.
    typedef struct two_bit {
        bool b1;
        bool b2;
    }two_Bit;

//    two_Bit ** BHT = (struct two_B it )
    two_Bit *ttBHT = (struct two_bit *) malloc(1024 * sizeof(struct two_bit));
    two_Bit *tfBHT = (struct two_bit *) malloc(1024 * sizeof(struct two_bit));
    two_Bit *ftBHT = (struct two_bit *) malloc(1024 * sizeof(struct two_bit));
    two_Bit *ffBHT = (struct two_bit *) malloc(1024 * sizeof(struct two_bit));


    memset(ttBHT,false,2048);
    memset(tfBHT,false,2048);
    memset(ftBHT,false,2048);
    memset(ffBHT,false,2048);


    bool prediction = false;

    /* Variable to store the the address of the branch. */
    uint32_t addr = 0;
    /*
     * Variable to store the actual branch result
     * (obtained from the predictor library)
     */
    bool actual = false;

    /*
     * Prediction loop, until tracefile is empty.
     */
    int flag = 0;

    while (predictor_getState() != DONE) {

        // get the previous 2 branches' addresses.

        /* Get the next branch address from the state machine. */
        if (predictor_getNextBranch(&addr) != 0) {
            fprintf(stderr, "ERROR: \"predictor_getNextBranch()\" called in "\
                   "a state it shouldn't be called!\n");
        }


        // making a 10 bit suffix.
        int index = addr & 0b1111111111; // current address is converted to a respective index.
        /* Do a prediction as it is in a state machine of 2 bits. */

        if(flag == 0){ // both previous branches are not taken.
            // do a 2 bit predictor on ffBHT.
            //flag = 0;
            if(!ffBHT[index].b1)
                prediction = false; // predict not taken.
            else
                prediction = true; // predict taken.
        }
        else if(flag == 1){ // immediate previous branch is only taken.
            // do a 2 bit predictor on ftBHT.
            //flag = 1;
            if(!ftBHT[index].b1)
                prediction = false; // predict not taken.
            else
                prediction = true; // predict taken.
        }
        else if(flag == 2){ // immediate previous branch is only not taken.
            // do a 2 bit predictor on tfBHT.
            //flag = 2;
            if(!tfBHT[index].b1)
                prediction = false; // predict not taken.
            else
                prediction = true; // predict taken.
        }
        else if(flag == 3){ // both previous branches are  taken.
            // do a 2 bit predictor on ttBHT.
            //flag = 3;
            if(!ttBHT[index].b1)
                prediction = false; // predict not taken.
            else
                prediction = true; // predict taken.
        }

        /*
         * Feed the prediction to the state machine, and get the actual
         * result back.
         */
        if (predictor_predict(prediction, &actual) != 0) {
            fprintf(stderr, "ERROR: \"predictor_predict()\" called in "\
                    " a state it shouldn't be called\n");
        }
        // store the actual state of the branch so that it can be used as the next prediction
        // if the branch is taken then change the states of the index.
        switch (flag){
            case 0:
                // ffBHT
                if(actual)// actually the branch is taken.
                    if(!ffBHT[index].b1 && !ffBHT[index].b2)
                        ffBHT[index].b2 = true; // from 00 -> 01
                    else
                        ffBHT[index].b1 = true,ffBHT[index].b2 = true;
                        // 01 -> 11 , 10 -> 11, 11 -> 11
                else // actually the branch is not taken.
                    if(ffBHT[index].b1 && ffBHT[index].b2)
                        ffBHT[index].b2 = false; // from 11 -> 10
                    else
                        ffBHT[index].b1 = false,ffBHT[index].b2 = false;
                        // 10 -> 00, 01 -> 00 , 00 -> 00
                break;
            case 1:
                //ftBHT
                if(actual)// actually the branch is taken.
                    if(!ftBHT[index].b1 && !ftBHT[index].b2)
                        ftBHT[index].b2 = true; // from 00 -> 01
                    else
                        ftBHT[index].b1 = true,ftBHT[index].b2 = true;
                    // 01 -> 11 , 10 -> 11, 11 -> 11
                else // actually the branch is not taken.
                    if(ftBHT[index].b1 && ftBHT[index].b2)
                        ftBHT[index].b2 = false; // from 11 -> 10
                    else
                        ftBHT[index].b1 = false,ftBHT[index].b2 = false;
                // 10 -> 00, 01 -> 00 , 00 -> 00
                break;
            case 2:
                //tfBHT
                if(actual)// actually the branch is taken.
                    if(!tfBHT[index].b1 && !tfBHT[index].b2)
                        tfBHT[index].b2 = true; // from 00 -> 01
                    else
                        tfBHT[index].b1 = true,tfBHT[index].b2 = true;
                    // 01 -> 11 , 10 -> 11, 11 -> 11
                else // actually the branch is not taken.
                    if(tfBHT[index].b1 && tfBHT[index].b2)
                        tfBHT[index].b2 = false; // from 11 -> 10
                    else
                        tfBHT[index].b1 = false,tfBHT[index].b2 = false;
                // 10 -> 00, 01 -> 00 , 00 -> 00
                break;
            case 3:
                //ttBHT
                if(actual)// actually the branch is taken.
                    if(!ttBHT[index].b1 && !ttBHT[index].b2)
                        ttBHT[index].b2 = true; // from 00 -> 01
                    else
                        ttBHT[index].b1 = true,ttBHT[index].b2 = true;
                    // 01 -> 11 , 10 -> 11, 11 -> 11
                else // actually the branch is not taken.
                    if(ttBHT[index].b1 && ttBHT[index].b2)
                        ttBHT[index].b2 = false; // from 11 -> 10
                    else
                        ttBHT[index].b1 = false,ttBHT[index].b2 = false;
                // 10 -> 00, 01 -> 00 , 00 -> 00
                break;

        }
        flag = flag << 1;
        flag = flag | (actual?1:0);
        flag = flag % 4;



    }
    free(ttBHT),free(tfBHT),free(ftBHT),free(ffBHT);


}
void custom_predictor(){

    printf("Custom Predictor \n" );
    // Create the data structure.
    typedef struct two_bit {
        bool b1;
        bool b2;
    }two_Bit;

    int correlated_bits = 7;
    int bht_size =  8192;
    int bhts_size = 1;
    for(int i=1;i<=correlated_bits;i++){
        bhts_size *= 2;
    }
    bht_size /= (bhts_size*2);


    two_Bit ** BHT = (struct two_bit **)malloc(sizeof(two_Bit *)*bhts_size);

    for(int i=0;i<bhts_size;i++){
        BHT[i] = (two_Bit *)malloc(bht_size*sizeof(two_Bit));
        memset(BHT[i],false,bht_size*sizeof(two_Bit));
    }

    bool prediction = false;

    /* Variable to store the the address of the branch. */
    uint32_t addr = 0;
    /*
     * Variable to store the actual branch result
     * (obtained from the predictor library)
     */
    bool actual = false;

    /*
     * Prediction loop, until tracefile is empty.
     */
    int flag = 0;

    while (predictor_getState() != DONE) {

        // get the previous 2 branches' addresses.
//        p_addr = addr;
//        pp_addr = p_addr;

        /* Get the next branch address from the state machine. */
        if (predictor_getNextBranch(&addr) != 0) {
            fprintf(stderr, "ERROR: \"predictor_getNextBranch()\" called in "\
                   "a state it shouldn't be called!\n");
        }

//        if(p_addr ==0){
//            p_addr = addr;
//        }
//        if(pp_addr == 0){
//            pp_addr = p_addr;
//        }
        // making a 10 bit suffix.
        int index  = addr & (bht_size-1);


//        int p_index = p_addr & 0b1111111111;
//        int pp_index = pp_addr & 0b1111111111;

        /* Do a prediction as it is in a state machine of 2 bits. */
        // first check for the branch histories.
//        bool p = pred_BHT[p_index];
//        bool pp = pred_BHT[pp_index];
        for(int i=0;i<bhts_size;i++){
            if(flag == i){
                if(!BHT[i][index].b1)
                    prediction = false; // predict not taken.
                else
                    prediction = true; // predict taken.
                break;
            }
        }
        // choose the corresponding table now.

        /*
         * Feed the prediction to the state machine, and get the actual
         * result back.
         */
        if (predictor_predict(prediction, &actual) != 0) {
            fprintf(stderr, "ERROR: \"predictor_predict()\" called in "\
                    " a state it shouldn't be called\n");
        }
        // store the actual state of the branch so that it can be used as the next prediction
        // if the branch is taken then change the states of the index.
        for(int i=0;i<bhts_size;i++){
            if(flag == i){
                if(actual)// actually the branch is taken.
                    if(!BHT[i][index].b1 && !BHT[i][index].b2)
                        BHT[i][index].b2 = true; // from 00 -> 01
                    else
                        BHT[i][index].b1 = true,BHT[i][index].b2 = true;
                    // 01 -> 11 , 10 -> 11, 11 -> 11
                else // actually the branch is not taken.
                if(BHT[i][index].b1 && BHT[i][index].b2)
                    BHT[i][index].b2 = false; // from 11 -> 10
                else
                    BHT[i][index].b1 = false,BHT[i][index].b2 = false;
                // 10 -> 00, 01 -> 00 , 00 -> 00
                break;
            }
        }


        flag = flag << 1;
        flag = flag | (actual?1:0);
        flag = flag % bhts_size;



    }
    free(BHT);

}

/* Bonus: Change these parameters to your needs */
void bonus_1() {
  always_x(false);
}

/* Bonus: Change these parameters to your needs */
void bonus_2() {
  always_x(false);
}
