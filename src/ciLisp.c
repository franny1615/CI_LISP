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

    bool found = false;
    int index = -1;

    while (!found) {
        index++;
        if (strcmp(funcNames[index], funcName) == 0) {
            found = true;
        }
    }

    switch (index) {
        case NEG_OPER:
            node->data.function.oper = NEG_OPER;
            break;
        case ABS_OPER:
            node->data.function.oper = ABS_OPER;
            break;
        case EXP_OPER:
            node->data.function.oper = EXP_OPER;
            break;
        case SQRT_OPER:
            node->data.function.oper = SQRT_OPER;
            break;
        case ADD_OPER:
            node->data.function.oper = ADD_OPER;
            break;
        case SUB_OPER:
            node->data.function.oper = SUB_OPER;
            break;
        case MULT_OPER:
            node->data.function.oper = MULT_OPER;
            break;
        case DIV_OPER:
            node->data.function.oper = DIV_OPER;
            break;
        case REMAINDER_OPER:
            node->data.function.oper = REMAINDER_OPER;
            break;
        case LOG_OPER:
            node->data.function.oper = LOG_OPER;
            break;
        case POW_OPER:
            node->data.function.oper = POW_OPER;
            break;
        case MAX_OPER:
            node->data.function.oper = MAX_OPER;
            break;
        case MIN_OPER:
            node->data.function.oper = MIN_OPER;
            break;
        case EXP2_OPER:
            node->data.function.oper = EXP2_OPER;
            break;
        case CBRT_OPER:
            node->data.function.oper = CBRT_OPER;
            break;
        case HYPOT_OPER:
            node->data.function.oper = HYPOT_OPER;
            break;
        case READ_OPER:
            node->data.function.oper = READ_OPER;
            break;
        case RAND_OPER:
            node->data.function.oper = RAND_OPER;
            break;
        case PRINT_OPER:
            node->data.function.oper = PRINT_OPER;
            break;
        case EQUAL_OPER:
            node->data.function.oper = EQUAL_OPER;
            break;
        case LESS_OPER:
            node->data.function.oper = LESS_OPER;
            break;
        case GREATER_OPER:
            node->data.function.oper = GREATER_OPER;
            break;
        default:
            break;
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
            break;
        case NUM_NODE_TYPE:
            result = evalNumNode(&node->data.number);
            break;
        case SYMBOL_NODE_TYPE:
            result = evalSymNode(&node->data.symbol, node);
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
                if (temp->val->type == FUNC_NODE_TYPE) {
                    result = evalFuncNode(&temp->val->data.function);
                } else {
                    result.type = temp->val->data.number.type;
                    result.value = temp->val->data.number.value;
                }
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
            default:
                break;
        }
    }
    if (numOfOperands == 2) {
        RET_VAL op1 = eval(funcNode->opList);
        RET_VAL op2 = eval(funcNode->opList->next);
        if (op1.type == DOUBLE_TYPE || op2.type == DOUBLE_TYPE) {
            result.type = DOUBLE_TYPE;
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
            default:
                break;
        }
    }
    if (numOfOperands >= 2) {
        AST_NODE *h = funcNode->opList;
        // need to do appropriate casting to result type
        while (h != NULL) {
            if (eval(h).type == DOUBLE_TYPE) { // only need one double to promote the result
                result.type = DOUBLE_TYPE;
                break;
            }
            h = h->next;
        }
        h = funcNode->opList;
        switch (funcNode->oper) {
            case ADD_OPER:
                // many operands
                result.value = 0;
                while (h != NULL) {
                    result.value += eval(h).value;
                    h = h->next;
                }
                break;
            case MULT_OPER:
                // multiplaction with many elements
                result.value = 1;
                while (h != NULL) {
                    result.value *= eval(h).value;
                    h = h->next;
                }
                break;
            case PRINT_OPER:
                printf("==> ");
                while (h != NULL) {
                    result = eval(h);
                    printRetVal(result);
                    printf(" ");
                    h = h->next;
                }
                printf("\n");
                break;
            default:
                break;
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

SYMBOL_TABLE_NODE *createSymbolTableNode(char *type, char *ident, AST_NODE *sexpr) {
    SYMBOL_TABLE_NODE *symbolTableNode = malloc(sizeof(SYMBOL_TABLE_NODE));
    symbolTableNode->ident = malloc(strlen(ident) * sizeof(char) + 1);
    strcpy(symbolTableNode->ident, ident);
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
    symbolTableNode->val = sexpr;
    symbolTableNode->next = NULL;
    return symbolTableNode;
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
    newItem->next = list;
    return newItem;// became the new head
}




