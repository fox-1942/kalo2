#include <draw.h>
#include "controller.h"

World world;
Rotate rotate;
Move move;
Camera camera;
Action action;
Data data;

void set_callbacks() {
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(key_handler);
    glutKeyboardUpFunc(key_up_handler);
    glutMouseFunc(mouse_handler);
    glutMotionFunc(motion_handler);
    glutIdleFunc(idle);
    glutSpecialFunc(specialFunc);
    glutTimerFunc(0, set_satellite_led_working_time, 0);
    if (FULLSCREEN == 1) { glutFullScreen(); }
}

void initialize() {
    data.speed = 70;
    data.angle = 135;

    set_callbacks();
    init_camera(&camera);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    glEnable(GL_AUTO_NORMAL);
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glClearDepth(1.0);
    glEnable(GL_LIGHTING);
    data.help = load_texture("..//textures//help.png");
    init_entities(&world);
    glEnable(GL_TEXTURE_2D);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);

    if (RESOLUTION == 1) { glutInitWindowSize(1920, 1080); }
    else { glutInitWindowSize(1366, 768); }
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    int wnd = glutCreateWindow("New Solar System");
    glutSetWindow(wnd);

    initialize();

    action.move_forward = FALSE;
    action.move_backward = FALSE;
    action.step_left = FALSE;
    action.step_right = FALSE;
    action.rotate_planets_in_galaxy = TRUE;
    action.call_satellite = FALSE;

    glutMainLoop();
    return 0;
}