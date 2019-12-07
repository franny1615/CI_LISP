#include "ciLisp.h"

void yyerror(char *s) {
    fprintf(stderr, "\nERROR: %s\n", s);
    // note stderr that normally defaults to stdout, but can be redirected: ./src 2> src.log
    // CLion will display stderr in a different color from stdin and stdout
}

// Array of string values for operations.
// Must be in sync with funcs in the OPER_TYPE enum in order for resolveFunc to work.
char *funcNames[] = {
        "neg",
        "abs",
        "exp",
        "sqrt",
        "add",
        "sub",
        "mult",
        "div",
        "remainder",
        "log",
        "pow",
        "max",
        "min",
        "exp2",
        "cbrt",
        "hypot",
        "read",
        "rand",
        "print",
        "equal",
        "less",
        "greater",
        ""
};

OPER_TYPE resolveFunc(char *funcName) {
    int i = 0;
    while (funcNames[i][0] != '\0') {
        if (strcmp(funcNames[i], funcName) == 0)
            return i;
        i++;
    }
    return CUSTOM_OPER;
}

// Called when an INT or DOUBLE token is encountered (see ciLisp.l and ciLisp.y).
// Creates an AST_NODE for the number.
// Sets the AST_NODE's type to number.
// Populates the value of the contained NUMBER_AST_NODE with the argument value.
// SEE: AST_NODE, NUM_AST_NODE, AST_NODE_TYPE.
AST_NODE *createNumberNode(double value, NUM_TYPE type) {
    AST_NODE *node;
    size_t nodeSize;

    // allocate space for the fixed sie and the variable part (union)
    nodeSize = sizeof(AST_NODE);
    if ((node = calloc(nodeSize, 1)) == NULL)
        yyerror("Memory allocation failed!");

    // TODO set the AST_NODE's type, assign values to contained NUM_AST_NODE
    node->type = NUM_NODE_TYPE;
    node->data.number.type = type;
    switch (type) {
        case INT_TYPE:
            node->data.number.value = value;
            break;
        case DOUBLE_TYPE:
            node->data.number.value = value;
            break;
        default:
            break;
    }

    return node;
}

// Called when an f_expr is created (see ciLisp.y).
// Creates an AST_NODE for a function call.
// Sets the created AST_NODE's type to function.
// Populates the contained FUNC_AST_NODE with:
//      - An OPER_TYPE (the enum identifying the specific function being called)
//      - 2 AST_NODEs, the operands
// SEE: AST_NODE, FUNC_AST_NODE, AST_NODE_TYPE.
//
AST_NODE *createFunctionNode(char *funcName, AST_NODE *op1) {
    AST_NODE *node;
    size_t nodeSize;

    // allocate space (or error)
    nodeSize = sizeof(AST_NODE);
    if ((node = calloc(nodeSize, 1)) == NULL)
        yyerror("Memory allocation failed!");

    // TODO set the AST_NODE's type, populate contained FUNC_AST_NODE
    // NOTE: you do not need to populate the "ident" field unless the function is type CUSTOM_OPER.
    // When you do have a CUSTOM_OPER, you do NOT need to allocate and strcpy here.
    // The funcName will be a string identifier for which space should be allocated in the tokenizer.
    // For CUSTOM_OPER functions, you should simply assign the "ident" pointer to the passed in funcName.
    // For functions other than CUSTOM_OPER, you should free the funcName after you're assigned the OPER_TYPE.

    node->type = FUNC_NODE_TYPE;

    node->data.function.oper = resolveFunc(funcName);
    if (node->data.function.oper == CUSTOM_OPER) {
        node->data.function.ident = malloc(sizeof(funcName));
        strcpy(node->data.function.ident, funcName);
    }

    node->data.function.opList = op1;

    AST_NODE *h = node->data.function.opList;
    while (h != NULL) {
        h->parent = node;
        h = h->next;
    }

    return node;
}

// Called after execution is done on the base of the tree.
// (see the program production in ciLisp.y)
// Recursively frees the whole abstract syntax tree.
// You'll need to update and expand freeNode as the project develops.
void freeNode(AST_NODE *node) {
    if (!node)
        return;

    if (node->type == FUNC_NODE_TYPE) {
        // Recursive calls to free child nodes
        freeNode(node->data.function.opList);
        // freeNode(node->data.function.op2);

        // Free up identifier string if necessary
        if (node->data.function.oper == CUSTOM_OPER) {
            free(node->data.function.ident);
        }
    }

    free(node);
}

// Evaluates an AST_NODE.
// returns a RET_VAL storing the the resulting value and type.
// You'll need to update and expand eval (and the more specific eval functions below)
// as the project develops.
RET_VAL eval(AST_NODE *node) {
    if (!node)
        return (RET_VAL) {INT_TYPE, NAN};

    RET_VAL result = {INT_TYPE, NAN}; // see NUM_AST_NODE, because RET_VAL is just an alternative name for it.

    // TODO complete the switch.
    // Make calls to other eval functions based on node type.
    // Use the results of those calls to populate result.
    switch (node->type) {
        case FUNC_NODE_TYPE:
            result = evalFuncNode(&node->data.function);
            if (node->data.function.oper == READ_OPER || node->data.function.oper == RAND_OPER) {
                node->type = NUM_NODE_TYPE;
                node->data.number = result;
            }
            break;
        case NUM_NODE_TYPE:
            result = evalNumNode(&node->data.number);
            break;
        case SYMBOL_NODE_TYPE:
            result = evalSymNode(&node->data.symbol, node);
            break;
        case COND_NODE_TYPE:
            result = evalConditionNode(&node->data.condition);
            break;
        default:
            yyerror("Invalid AST_NODE_TYPE, probably invalid writes somewhere!");
    }

    return result;
}

RET_VAL evalSymNode(SYMBOL_AST_NODE *symbolNode, AST_NODE *parent) {
    RET_VAL result = {INT_TYPE, NAN};

    SYMBOL_TABLE_NODE *temp = parent->symbolTable;
    AST_NODE *pnt = parent->parent;
    bool found = false;

    while (!found) {
        if (temp == NULL) { // wasn't found in this scope, go up a scope, look for the var there
            if (parent->parent != NULL) {
                temp = pnt->symbolTable;
                pnt = pnt->parent;
            } else {
                break;
            }
        } else {
            if (strcmp(symbolNode->ident, temp->ident) == 0) {
                result = eval(temp->val);
                if (temp->val_type == INT_TYPE && result.type == DOUBLE_TYPE) {
                    result.value = (int) result.value;
                    result.type = INT_TYPE;
                    printf("!!!Warning!!! Precision loss with assignment to %s\n", temp->ident);
                }
                if (temp->val_type == DOUBLE_TYPE && result.type == INT_TYPE) {
                    result.type = DOUBLE_TYPE;
                }
                found = true;
            }
            temp = temp->next;
        }
    }

    return result;
}

// returns a pointer to the NUM_AST_NODE (aka RET_VAL) referenced by node.
// DOES NOT allocate space for a new RET_VAL.
RET_VAL evalNumNode(NUM_AST_NODE *numNode) {
    if (!numNode)
        return (RET_VAL) {INT_TYPE, NAN};

    RET_VAL result = {INT_TYPE, NAN};

    // TODO populate result with the values stored in the node.
    // SEE: AST_NODE, AST_NODE_TYPE, NUM_AST_NODE

    result.type = numNode->type;
    switch (numNode->type) {
        case INT_TYPE:
            result.value = numNode->value;
            break;
        case DOUBLE_TYPE:
            result.value = numNode->value;
            break;
    }

    return result;
}

RET_VAL evalConditionNode(COND_AST_NODE *conditionNode) {
    RET_VAL result = {INT_TYPE, NAN};

    RET_VAL cond = eval(conditionNode->cond);
    if (cond.value != 0) {
        result = eval(conditionNode->tru);
    } else {
        result = eval(conditionNode->fls);
    }

    return result;
}


RET_VAL evalFuncNode(FUNC_AST_NODE *funcNode) {
    if (!funcNode)
        return (RET_VAL) {INT_TYPE, NAN};

    RET_VAL result = {INT_TYPE, NAN};

    // TODO populate result with the result of running the function on its operands.
    // SEE: AST_NODE, AST_NODE_TYPE, FUNC_AST_NODE

    int numOfOperands = 0;
    AST_NODE *head = funcNode->opList;
    while (head != NULL) {
        numOfOperands++;
        head = head->next;
    }

    if (funcNode->oper == CUSTOM_OPER) {
        // look at the oplist parents parent symbol table
        // check that the symbol table node is of lambda type
        // check if the ident matches with the lambda ident
        AST_NODE *parent = NULL;
        if (funcNode->opList == NULL) {
            printf("!!! ERROR !!! Please provide actual parameters on next attempt\n");
            result.value = 0.0;
        } else {
            parent = funcNode->opList->parent;
        }

        SYMBOL_TABLE_NODE *h = NULL;
        while (parent != NULL) {
            if (parent->symbolTable != NULL) {
                h = parent->symbolTable;
                break;
            }
            parent = parent->parent;
        }
        while (h != NULL) {
            if (h->type == LAMBDA_TYPE) {
                if (strcmp(h->ident, funcNode->ident) == 0) {
                    // first resolve the parameter values
                    SYMBOL_TABLE_NODE *arg = h->val->symbolTable;
                    AST_NODE *plist = funcNode->opList;
                    while (arg != NULL) {
                        AST_NODE *argResolved = malloc(sizeof(AST_NODE));
                        argResolved->type = NUM_NODE_TYPE;
                        if (plist == NULL) {
                            printf("!!! ERROR !!! too few operands were given, terminating...\n");
                            break;
                        }
                        argResolved->data.number = eval(plist);
                        arg->val = argResolved;
                        arg = arg->next;
                        plist = plist->next;
                        if (arg == NULL && plist != NULL) {
                            printf("!!! WARNING !!! too many operands were given for the custom function\n");
                        }
                    }
                    // then use those values to do the definition
                    result = eval(h->val);
                    result.type = h->val_type;
                }
            }
            h = h->next;
        }
    } else {
        if (numOfOperands == 0) {
            switch (funcNode->oper) {
                case RAND_OPER:
                    result.type = DOUBLE_TYPE;
                    result.value = ((double) rand()) / RAND_MAX;
                    break;
                case READ_OPER:
                    result = readNum();
                    break;
                default:
                    break;
            }
        }

        if (numOfOperands == 1) {
            result = eval(funcNode->opList);
            switch (funcNode->oper) {
                case NEG_OPER:
                    //one element
                    result.value = -result.value;
                    break;
                case ABS_OPER:
                    //abs(a)
                    result.value = fabs(result.value);
                    break;
                case EXP_OPER:
                    //exp(a)
                    result.value = exp(result.value);
                    break;
                case SQRT_OPER:
                    //sqrt(a)
                    result.value = sqrt(result.value);
                    break;
                case LOG_OPER:
                    // log(a)
                    result.value = log(result.value);
                    break;
                case EXP2_OPER:
                    // exp2(a) ... 2 to the power of a
                    result.value = exp2(result.value);
                    break;
                case CBRT_OPER:
                    // cbrt(a)
                    result.value = cbrt(result.value);
                    break;
                case PRINT_OPER:
                    printf("==> ");
                    printRetVal(result);
                    printf("\n");
                    break;
                default:
                    printf("ERROR: too few parameters provided for the function: %s", funcNames[funcNode->oper]);
                    break;
            }
        }
        if (numOfOperands >= 2) {
            RET_VAL op1 = {INT_TYPE, NAN};
            RET_VAL op2 = {INT_TYPE, NAN};
            if (numOfOperands == 2) {
                op1 = eval(funcNode->opList);
                op2 = eval(funcNode->opList->next);
                if (op1.type == DOUBLE_TYPE || op2.type == DOUBLE_TYPE) {
                    result.type = DOUBLE_TYPE;
                }
            }

            AST_NODE *h = funcNode->opList;


            if (numOfOperands > 2 &&
                (funcNode->oper == SUB_OPER || funcNode->oper == DIV_OPER || funcNode->oper == REMAINDER_OPER ||
                 funcNode->oper == POW_OPER || funcNode->oper == MAX_OPER || funcNode->oper == MIN_OPER ||
                 funcNode->oper == HYPOT_OPER)) {
                printf("WARNING: more than needed parameters passed in for function: %s\n", funcNames[funcNode->oper]);
            }
            switch (funcNode->oper) {
                case SUB_OPER:
                    // only two elements
                    result.value = op1.value - op2.value;
                    break;
                case DIV_OPER:
                    // a/b only two operands
                    result.value = op1.value / op2.value;
                    break;
                case REMAINDER_OPER:
                    result.value = fmod(op1.value, op2.value);
                    //fmod(a,b)
                    break;
                case POW_OPER:
                    // pow(a,b)
                    result.value = pow(op1.value, op2.value);
                    break;
                case MAX_OPER:
                    //fmax(a,b)
                    result.value = fmax(op1.value, op2.value);
                    break;
                case MIN_OPER:
                    // fmin(a,b)
                    result.value = fmin(op1.value, op2.value);
                    break;
                case HYPOT_OPER:
                    // hypot(a,b)
                    result.value = hypot(op1.value, op2.value);
                    break;
                case ADD_OPER:
                    // many operands
                    result.value = 0;
                    if (numOfOperands == 2) {
                        result.value = op1.value + op2.value;
                    } else {
                        while (h != NULL) {
                            RET_VAL temp = eval(h);
                            if (temp.type == DOUBLE_TYPE) {
                                result.type = DOUBLE_TYPE;
                            }
                            result.value += temp.value;
                            h = h->next;
                        }
                    }
                    break;
                case LESS_OPER:
                    if (op1.value < op2.value) {
                        result.value = 1;
                    } else {
                        result.value = 0;
                    }
                    break;
                case GREATER_OPER:
                    if (op1.value > op2.value) {
                        result.value = 1;
                    } else {
                        result.value = 0;
                    }
                    break;
                case EQUAL_OPER:
                    if (op1.value == op2.value) {
                        result.value = 1;
                    } else {
                        result.value = 0;
                    }
                    break;
                case MULT_OPER:
                    // multiplaction with many elements
                    result.value = 1;
                    if (numOfOperands == 2) {
                        result.value = op1.value * op2.value;
                    } else {
                        while (h != NULL) {
                            RET_VAL temp = eval(h);
                            if (temp.type == DOUBLE_TYPE) {
                                result.type = DOUBLE_TYPE;
                            }
                            result.value *= temp.value;
                            h = h->next;
                        }
                    }
                    break;
                case PRINT_OPER:
                    if (numOfOperands == 2) {
                        printf("==> ");
                        printRetVal(op1);
                        printf(" ");
                        printRetVal(op2);
                        result = op2;
                    } else {
                        RET_VAL values[numOfOperands];
                        for (int i = 0; i < numOfOperands; i++) {
                            values[i] = eval(h);
                            h = h->next;
                        }
                        printf("==> ");
                        for (int i = 0; i < numOfOperands; i++) {
                            printRetVal(values[i]);
                            printf(" ");
                        }
                        result = values[numOfOperands - 1];
                    }
                    printf("\n");
                    break;
                default:
                    break;
            }
        }
    }

    return result;
}

// prints the type and value of a RET_VAL
void printRetVal(RET_VAL val) {
    // TODO print the type and value of the value passed in.
    switch (val.type) {
        case INT_TYPE:
            printf("%d", (int) val.value);
            break;
        case DOUBLE_TYPE:
            printf("%.2f", val.value);
            break;
    }
}

AST_NODE *createSymbolNode(char *ident) {
    AST_NODE *symbolNode = malloc(sizeof(SYMBOL_AST_NODE));
    symbolNode->type = SYMBOL_NODE_TYPE;
    symbolNode->parent = NULL;
    symbolNode->symbolTable = NULL;
    symbolNode->data.symbol.ident = malloc(strlen(ident) * sizeof(char));
    strcpy(symbolNode->data.symbol.ident, ident);
    return symbolNode;
}

// assigning the symbol table that we are passed in to the ast node
AST_NODE *linkSymbolTable(SYMBOL_TABLE_NODE *symTable, AST_NODE *node) {
    if (node != NULL) {
        node->symbolTable = symTable;
    }
    return node;
}

// return the new head of the list
SYMBOL_TABLE_NODE *addAtHead(SYMBOL_TABLE_NODE *list, SYMBOL_TABLE_NODE *element) {
    element->next = list;
    return element;
}

AST_NODE *linkFunNodeList(AST_NODE *newItem, AST_NODE *list) {
    if (newItem != NULL) {
        newItem->next = list;
    }
    return newItem;// became the new head
}

AST_NODE *createConditionalNode(AST_NODE *condition, AST_NODE *doOne, AST_NODE *doZero) {
    COND_AST_NODE *conditionNode = malloc(sizeof(COND_AST_NODE));
    conditionNode->cond = condition;
    conditionNode->tru = doOne;
    conditionNode->fls = doZero;


    AST_NODE *cN = malloc(sizeof(AST_NODE));
    cN->type = COND_NODE_TYPE;
    cN->data.condition = *conditionNode;
    cN->parent = NULL;
    cN->next = NULL;
    cN->symbolTable = NULL;

    cN->data.condition.cond->parent = cN;
    cN->data.condition.tru->parent = cN;
    cN->data.condition.fls->parent = cN;

    return cN;
}

RET_VAL readNum() {
    printf("read := ");
    char input[10];
    scanf("%s", input);
    getchar();

    char num[10];
    bool decimal = false;
    int i;
    for (i = 0; i < strlen(input); i++) {
        if (!decimal && input[i] == '.') {
            decimal = true;
            num[i] = input[i];
        }
        if (isdigit(input[i])) {
            num[i] = input[i];
        }
    }
    if (i == 9) {
        num[9] = '\0';
    } else {
        num[i] = '\0';
    }

    RET_VAL res = {INT_TYPE, NAN};
    if (decimal) {
        res.type = DOUBLE_TYPE;
    }
    char **junk = 0;
    res.value = strtod(num, junk);

    return res;
}

SYMBOL_TABLE_NODE *linkArgList(char *addition, SYMBOL_TABLE_NODE *list) {
    SYMBOL_TABLE_NODE *new = malloc(sizeof(SYMBOL_TABLE_NODE));
    new->next = list;
    new->type = ARG_TYPE;
    new->val_type = DOUBLE_TYPE;
    new->ident = malloc(sizeof(strlen(addition)));
    strcpy(new->ident, addition);
    new->val = NULL;
    new->stack = NULL;
    return new;
}

SYMBOL_TABLE_NODE *createFuncNode(char *type, char *identifier, SYMBOL_TABLE_NODE *args, AST_NODE *sexpr) {
    SYMBOL_TABLE_NODE *symbolTableNode = malloc(sizeof(SYMBOL_TABLE_NODE));
    symbolTableNode->ident = malloc(strlen(identifier) * sizeof(char) + 1);
    strcpy(symbolTableNode->ident, identifier);
    if (args == NULL) {
        symbolTableNode->type = VARIABLE_TYPE;
    } else {
        symbolTableNode->type = LAMBDA_TYPE;
    }
    if (type != NULL) {
        if (strcmp(type, "int") == 0) {
            symbolTableNode->val_type = INT_TYPE;
        }
        if (strcmp(type, "double") == 0) {
            symbolTableNode->val_type = DOUBLE_TYPE;
        }
    } else {
        symbolTableNode->val_type = DOUBLE_TYPE;
    }
    symbolTableNode->stack = NULL;
    symbolTableNode->val = sexpr;
    symbolTableNode->val->symbolTable = args;
    symbolTableNode->next = NULL;

    return symbolTableNode;
}

