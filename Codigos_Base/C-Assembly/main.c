#include <stdio.h>

extern float calc_circ_area(float raio);
extern float calc_cone_vol(float area, float h);

int main() {
    float r, h, area, volume;
    printf("Digite o raio e a altura do cone \n");
    scanf("%f %f", &r, &h);

    area = calc_circ_area(r);

    printf("Area de circ com raio da base eh igual a: %f \n", area);

    volume = calc_cone_vol(area, h);
    printf("O volume do cone eh igual a: %f \n", volume);

    return 0;

}

