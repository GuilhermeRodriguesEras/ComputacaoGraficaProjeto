#include <GL/glut.h>
#include <math.h> 

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

GLuint idsTextura[2];
GLUquadric* quad;
float distanciaX = 15.0, alturaCam = 3.0;
float angulo = 0.0;
float posX = distanciaX * cos(angulo);
float posZ = distanciaX * sin(angulo);
float braco1x = -15, braco1y = 0, braco1z = 0;
float braco2x = -100, braco2y = 0, braco2z = 0;
float braco3x = 90, braco3y = 0, braco3z = 0;
float braco4x = 37, braco4y = 0, braco4z = 0;
float conesX = 50, conesY = 0, conesZ = 0;
float lightIntensite = 0.4;
bool animacaoOn = false;

void carregarTextura(const char* nomeArquivo, int indice){
    int largura, altura, canais;

    unsigned char *dados = stbi_load(nomeArquivo, &largura, &altura, &canais, 0);

    if (!dados)
        exit(1);

    // gerar textura
    glGenTextures(1, &idsTextura[indice]);
    glBindTexture(GL_TEXTURE_2D, idsTextura[indice]);

    // configurar parametros da textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, (canais == 4) ? GL_RGBA : GL_RGB, 
                largura, altura, 0, (canais == 4) ? GL_RGBA : GL_RGB,
                GL_UNSIGNED_BYTE, dados);

    // liberar a memoria da imagem
    stbi_image_free(dados);              
}

void drawCilindro(float size){
    glPushMatrix();
    glRotatef(90, -1.0, 0, 0.0);
    glScaled(0.5*size, 0.5*size, 2*size);
    quad = gluNewQuadric();
    gluCylinder(quad, 0.5, 0.5, 1.0, 32, 16);
    glPopMatrix();
}

void rotatedBracos(float x, float y, float z){
    glRotatef(x, 1, 0, 0);
    glRotatef(y, 0, 1, 0);
    glRotatef(z, 0, 0, 1);
}

void inicializa() {
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glEnable(GL_DEPTH_TEST);

    carregarTextura("texturas/chao.jpg", 0);
    carregarTextura("texturas/parede.jpg", 1);
    carregarTextura("texturas/teto.jpg", 2);
}

void drawHouse(float size){

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // configurar a luz do sol
    GLfloat luzAmbiente[] = {lightIntensite, lightIntensite, lightIntensite, 1.0};   // luz suave
    GLfloat luzDifusa[] = {1.0, 1.0, 1.0, 1.0};     // luz branca
    GLfloat luzPosicao[] = {0.0, 6.0, 0.0, 1.0};    // luz pontual

    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
    glLightfv(GL_LIGHT0, GL_POSITION, luzPosicao);

    glEnable(GL_COLOR_MATERIAL); // permite o controle da cor com glColor3f
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    glEnable(GL_TEXTURE_2D);
    // floor
    glBindTexture(GL_TEXTURE_2D, idsTextura[0]);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex3f(-size, -5, -size);
        glTexCoord2f(1.0, 0.0); glVertex3f(size, -5, -size);
        glTexCoord2f(1.0, 1.0); glVertex3f(size, -5, size);
        glTexCoord2f(0.0, 1.0); glVertex3f(-size, -5, size);
    glEnd();

    // teto
    glBindTexture(GL_TEXTURE_2D, idsTextura[2]);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex3f(-size, size, -size);
        glTexCoord2f(1.0, 0.0); glVertex3f(size, size, -size);
        glTexCoord2f(1.0, 1.0); glVertex3f(size, size, size);
        glTexCoord2f(0.0, 1.0); glVertex3f(-size, size, size);
    glEnd();

    // paredes
    glBindTexture(GL_TEXTURE_2D, idsTextura[1]);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex3f(-size, -size, size);
        glTexCoord2f(1.0, 0.0); glVertex3f(size, -size, size);
        glTexCoord2f(1.0, 1.0); glVertex3f(size, size, size);
        glTexCoord2f(0.0, 1.0); glVertex3f(-size, size, size);

        glTexCoord2f(0.0, 0.0); glVertex3f(-size, -size, -size);
        glTexCoord2f(1.0, 0.0); glVertex3f(size, -size, -size);
        glTexCoord2f(1.0, 1.0); glVertex3f(size, size, -size);
        glTexCoord2f(0.0, 1.0); glVertex3f(-size, size, -size);

        glTexCoord2f(0.0, 0.0); glVertex3f(size, -size, -size);
        glTexCoord2f(1.0, 0.0); glVertex3f(size, size, -size);
        glTexCoord2f(1.0, 1.0); glVertex3f(size, size, size);
        glTexCoord2f(0.0, 1.0); glVertex3f(size, -size, size);

        glTexCoord2f(0.0, 0.0); glVertex3f(-size, -size, -size);
        glTexCoord2f(1.0, 0.0); glVertex3f(-size, size, -size);
        glTexCoord2f(1.0, 1.0); glVertex3f(-size, size, size);
        glTexCoord2f(0.0, 1.0); glVertex3f(-size, -size, size);
    glEnd();

    size -= 0.05;
    // rodape
    glBindTexture(GL_TEXTURE_2D, idsTextura[0]);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex3f(-size, -size, size);
        glTexCoord2f(1.0, 0.0); glVertex3f(size, -size, size);
        glTexCoord2f(1.0, 1.0); glVertex3f(size, -4, size);
        glTexCoord2f(0.0, 1.0); glVertex3f(-size, -4, size);

        glTexCoord2f(0.0, 0.0); glVertex3f(-size, -size, -size);
        glTexCoord2f(1.0, 0.0); glVertex3f(size, -size, -size);
        glTexCoord2f(1.0, 1.0); glVertex3f(size, -4, -size);
        glTexCoord2f(0.0, 1.0); glVertex3f(-size, -4, -size);

        glTexCoord2f(0.0, 0.0); glVertex3f(size, -size, -size);
        glTexCoord2f(1.0, 0.0); glVertex3f(size, -4, -size);
        glTexCoord2f(1.0, 1.0); glVertex3f(size, -4, size);
        glTexCoord2f(0.0, 1.0); glVertex3f(size, -size, size);

        glTexCoord2f(0.0, 0.0); glVertex3f(-size, -size, -size);
        glTexCoord2f(1.0, 0.0); glVertex3f(-size, -4, -size);
        glTexCoord2f(1.0, 1.0); glVertex3f(-size, -4, size);
        glTexCoord2f(0.0, 1.0); glVertex3f(-size, -size, size);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    // braço
    glPushAttrib(GL_CURRENT_BIT);

        glColor3f(0.78, 0.78, 0.78);
        glPushMatrix();
            glTranslatef(0.0, 0.375, 0);

            // primeiro cilindro
            drawCilindro(1);

            glTranslatef(0.0, 2.0, 0.0);
            glColor3f(0.447, 0.71, 0.949);
            glutSolidSphere(0.35, 50, 50);
            
            glPushMatrix();

                glColor3f(0.78, 0.78, 0.78);
                rotatedBracos(braco1x, braco1y, braco1z);

                // segundo cilindro
                drawCilindro(1);

                glTranslatef(0.0, 2.0, 0.0);
                glColor3f(0.447, 0.71, 0.949);
                glutSolidSphere(0.35, 50, 50);

                glPushMatrix();

                    glColor3f(0.78, 0.78, 0.78);
                    rotatedBracos(braco2x, braco2y, braco2z);

                    // terceiro cilindro
                    drawCilindro(1);

                    glTranslatef(0.0, 2.0, 0.0);
                    glColor3f(0.447, 0.71, 0.949);
                    glutSolidSphere(0.35, 50, 50);

                    glPushMatrix();

                        glColor3f(0.65, 0, 0);
                        rotatedBracos(braco3x, braco3y, braco3z);

                        drawCilindro(0.5);

                        glTranslatef(0.0, 1.0, 0.0);
                        glColor3f(0.78, 0.78, 0.78);
                        glutSolidSphere(0.17, 50, 50);

                        glColor3f(0.65, 0, 0);
                        glPushMatrix();
                            rotatedBracos(braco4x, braco4y, braco4z);
                            drawCilindro(0.5);

                            glTranslatef(0.0, 1.0, 0.0);
                            glColor3f(0.78, 0.78, 0.78);
                            glutSolidSphere(0.17, 50, 50);
                            glColor3f(0.65, 0, 0);
                            glPushMatrix();
                                glRotatef(-90, 1, 0, 0);
                                rotatedBracos(-conesX, conesY, conesZ);
                                glutSolidCone(0.12, 1, 20, 20);
                            glPopMatrix();

                        glPopMatrix();
                        
                        glPushMatrix();
                            rotatedBracos(-braco4x, braco4y, braco4z);
                            drawCilindro(0.5);  

                            glTranslatef(0.0, 1.0, 0.0);
                            glColor3f(0.78, 0.78, 0.78);
                            glutSolidSphere(0.17, 50, 50);
                            glColor3f(0.65, 0, 0);
                            glPushMatrix();
                                glRotatef(-90, 1, 0, 0);
                                rotatedBracos(conesX, conesY, conesZ);
                                glutSolidCone(0.12, 1, 20, 20);
                            glPopMatrix();

                        glPopMatrix();

                    glPopMatrix();

                glPopMatrix();

            glPopMatrix();

        glPopMatrix();


    glPopAttrib();

    // table 
    glPushAttrib(GL_CURRENT_BIT);
    glColor3f(0.478, 0.51, 0.373);

        glPushMatrix();
            glTranslatef(0.0, -2.5, 0.0);
            glScalef(0.75, 5.0, 0.75);
            
            glTranslatef(-5.0,0.0,-5.0);
            glutSolidCube(1);

            glTranslatef(10.0, 0.0, 0.0);
            glutSolidCube(1);

            glTranslatef(0.0, 0.0, 10.0);
            glutSolidCube(1);

            glTranslatef(-10.0, 0.0, 0.0);
            glutSolidCube(1);
        glPopMatrix();
        glPushMatrix();
            glScalef(10.0, 0.75, 10.0);
            glutSolidCube(1);
        glPopMatrix();
    glPopAttrib();
}

void voltarCameraPosInicial(){
    distanciaX = 15.0, alturaCam = 3.0;
    angulo = 0.0;
    posX = distanciaX * cos(angulo);
    posZ = distanciaX * sin(angulo);
}

void voltarArticulacaoPosInicial(){
    braco1x = -15, braco1y = 0, braco1z = 0;
    braco2x = -100, braco2y = 0, braco2z = 0;
    braco3x = 90, braco3y = 0, braco3z = 0;
    braco4x = 37, braco4y = 0, braco4z = 0;
    conesX = 50, conesY = 0, conesZ = 0;
}

void voltarLuzInicial(){
    lightIntensite = 0.4;
}

void resetTudo(){
    voltarArticulacaoPosInicial();
    voltarCameraPosInicial();
    voltarLuzInicial();
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    gluLookAt(posX, alturaCam, posZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    drawHouse(20);

    glutSwapBuffers();
}

void reshape(int largura, int altura){
    glViewport(0, 0, largura, altura);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65.0, (GLfloat)largura/(GLfloat)altura, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
     
}

void teclado(unsigned char tecla, int x, int y){
    if ((tecla == 'W' || tecla == 'w') && distanciaX >= 7){
        distanciaX -= 1.0;
        posX = distanciaX * cos(angulo); 
        posZ = distanciaX * sin(angulo);
    }
    if ((tecla == 'S' || tecla == 's') && distanciaX <= 19.0){
        distanciaX += 1.0; 
        posX = distanciaX * cos(angulo); 
        posZ = distanciaX * sin(angulo);
    }
    if (tecla == 'A' || tecla == 'a'){
        angulo += 0.05;
        if(angulo >= 360) angulo = 0;
        posX = distanciaX * cos(angulo); 
        posZ = distanciaX * sin(angulo);
        animacaoOn = false;
    }
    if (tecla == 'd' || tecla == 'D'){
        angulo -= 0.05;
        if(angulo <= -360) angulo = 0;
        posX = distanciaX * cos(angulo); 
        posZ = distanciaX * sin(angulo);
        animacaoOn = false;
    }
    if(tecla == ' '){
        braco4x = 29; conesX = 57;
    }
    if (tecla == '+' && alturaCam <= 6.0)
        alturaCam += 0.5;

    if (tecla == '-' && alturaCam >= -4.0)
        alturaCam -= 0.5;
    if(tecla == '1' && lightIntensite < 1) 
        lightIntensite += 0.05;
    if(tecla == '2' && lightIntensite > 0)
        lightIntensite -= 0.05;
        
    if(tecla == 'r') braco1x += 2;
    if(tecla == 'R') braco1x -= 2;
    if(tecla == 'f') braco1y += 2;
    if(tecla == 'F') braco1y -= 2;
    if(tecla == 'v') braco1z += 2;
    if(tecla == 'V') braco1z -= 2;
    if(tecla == 't') braco2x += 2;
    if(tecla == 'T') braco2x -= 2;
    if(tecla == 'g') braco2y += 2;
    if(tecla == 'G') braco2y -= 2;
    if(tecla == 'b') braco2z += 2;
    if(tecla == 'B') braco2z -= 2;
    if(tecla == 'y') braco3x += 2;
    if(tecla == 'Y') braco3x -= 2;
    if(tecla == 'h') braco3y += 2;
    if(tecla == 'H') braco3y -= 2;
    if(tecla == 'n') braco3z += 2;
    if(tecla == 'N') braco3z -= 2;

    //if(tecla == '1') luz += 0.1;

    glutPostRedisplay();
}

void menu(int opcao){
    switch(opcao){
        case 1: resetTudo(); break;
        case 2: voltarArticulacaoPosInicial(); break;
        case 3: voltarLuzInicial(); break;
        case 4: voltarCameraPosInicial(); break;
        case 5: if(lightIntensite < 1) lightIntensite += 0.05; break;
        case 6: if(lightIntensite > 0) lightIntensite -= 0.05; break;
        case 7: animacaoOn = true; break;
        case 8: animacaoOn = false; break;
    }

    glutPostRedisplay();
}

void tecladoUp(unsigned char tecla, int x, int y){
    if(tecla == ' '){
        braco4x = 37; conesX = 50;
    }
    glutPostRedisplay();
}

void animacao(int valor){
    if(animacaoOn == true){
        angulo -= 0.01;
        if(angulo <= -360) angulo = 0;
        posX = distanciaX * cos(angulo); 
        posZ = distanciaX * sin(angulo);
    }
    glutPostRedisplay();
    glutTimerFunc(16, animacao, 0);   
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Casa");

    inicializa();

    glutDisplayFunc(display);

    int subMenuReset = glutCreateMenu(menu);
    glutAddMenuEntry("Resetar tudo", 1);
    glutAddMenuEntry("Resetar Articulacoes", 2);
    glutAddMenuEntry("Restar Iluminacao", 3);
    glutAddMenuEntry("Resetar Camera", 4);

    int subMenuLuz = glutCreateMenu(menu);
    glutAddMenuEntry("Aumentar luz", 5);
    glutAddMenuEntry("Reduzir luz", 6);

    glutCreateMenu(menu);
    glutAddSubMenu("Resetar", subMenuReset);
    glutAddSubMenu("Iluminacao", subMenuLuz);
    glutAddMenuEntry("Ligar Camera rotativa", 7);
    glutAddMenuEntry("Desligar Camera rotativa", 8);

    glutAttachMenu(GLUT_MIDDLE_BUTTON);

    glutReshapeFunc(reshape);
    glutKeyboardFunc(teclado);
    glutKeyboardUpFunc(tecladoUp);

    glutTimerFunc(16, animacao, 0);  
    
    glutMainLoop();

    return 0;
}