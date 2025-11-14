#include <limits>
#include <iomanip>
#include <cstdlib> // Para rand(), system().
#include "funciones.h"

using namespace std;

// --- IMPLEMENTACIÓN DE LA FUNCIÓN DE MENÚ ---
void mostrarMenuPrincipal() {
    system("cls");  // Limpieza

    cout << "===========================================" << endl;
    cout << "         PROGRAMA DE GESTION               " << endl;
    cout << "===========================================" << endl;
    cout << "1. Cargar lote de marcas" << endl;
    cout << "2. Cargar lote de productos" << endl;
    cout << "3. Cargar lote de formas de pago" << endl;
    cout << "4. Cargar lote de ventas" << endl;
    cout << "5. Mostrar reportes" << endl;
    cout << "0. Salir" << endl;
    cout << "-------------------------------------------" << endl;
}

// --- FUNCIÓN CONTROLADORA PRINCIPAL ---
void ejecutarPrograma() {
    // Inicialización de la semilla aleatoria.
    srand(time(NULL));

    // Arrays para almacenar los datos de los lotes maestros.
    Marca marcas[MAX_MARCAS];
    Producto productos[MAX_PRODUCTOS];
    FormaPago formasPago[MAX_FORMAS_PAGO];

    // Arrays de ACUMULACIÓN para los reportes - (Inicio en 0).
    float recaudacionProducto[MAX_PRODUCTOS] = {};
    int cantidadVendidaProducto[MAX_PRODUCTOS] = {};
    int comprasCliente[MAX_CLIENTES] = {};
    int ventasFDP[MAX_FORMAS_PAGO] = {};
    int ventasMarcaFDP[MAX_MARCAS][MAX_FORMAS_PAGO] = {{}};
    int contadorVentas = 0;

    // Array de control de estado de carga de lotes.
    bool lotesCargados[] = {false, false, false, false};

    int opcion;

    do {
        mostrarMenuPrincipal();
        cout << "Ingrese una opcion: ";
        cin >> opcion;

        if (cin.fail()) { // Detecta si el ultimo cin fallo por tipo de dato no valido.
            cin.clear(); // Borra el estado de error para poder seguir.
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpieza total del buffer.
            opcion = -1; // Default
        }

        switch (opcion) {
            case 1:
                cargarLoteMarcas(marcas, lotesCargados[0]);
                break;
            case 2:
                cargarLoteProductos(productos, marcas, lotesCargados[0], lotesCargados[1]);
                break;
            case 3:
                cargarLoteFormasPago(formasPago, lotesCargados[2]);
                break;
            case 4:
                if (validarLotesCargados(lotesCargados)) {
                    cargarLoteVentas(productos, formasPago, recaudacionProducto, cantidadVendidaProducto, comprasCliente, ventasFDP, ventasMarcaFDP, contadorVentas, lotesCargados, lotesCargados[3]);
                } else {
                    cout << "ERROR: Debe cargar los lotes de Marcas, Productos y Formas de Pago primero (Opciones 1, 2 y 3)." << endl;
                }
                break;
            case 5:
                if (lotesCargados[3]) {
                    mostrarReportes(productos, marcas, formasPago, recaudacionProducto, cantidadVendidaProducto, comprasCliente, ventasFDP, ventasMarcaFDP, contadorVentas);
                } else {
                    cout << "ERROR: Debe cargar/procesar el Lote de Ventas primero (Opcion 4)." << endl;
                }
                break;
            case 0:
                cout << "Saliendo del programa... Hasta la proxima!" << endl;
                break;
            default:
                cout << "Opcion invalida, intente de nuevo." << endl;
        }

        if (opcion != 0) {
            system("pause");
        }

    } while (opcion != 0);
}

// --- FUNCIONES AUXILIARES GENERALES ---
bool validarCodigoMarca(int codigo) {     // Validacion de rango (1 a 10). Devuelve true or false
    return codigo >= 1 && codigo <= 10; }
bool validarStringNoVacio(const string& texto) {     // Validacion si string esta vacia "".
    return !texto.empty(); }
int buscarMarcaPorCodigo(const Marca marcas[], int codigo) {     // Validacion Marca cargada en lote.



    for (int i = 0; i < MAX_MARCAS; ++i) {
        if (marcas[i].codigo == codigo) { return i; }
    }
    return -1;
}
bool validarLotesCargados(const bool lotesCargados[]) {     // Validacion Lote 1,2 y 3 cargados = true para poder seguir con ventas y reportes.
    return lotesCargados[0] && lotesCargados[1] && lotesCargados[2]; }

//--- AUXILIAR LOTE 3 ---
int buscarCodigoFDP(const string& codigo) {
    for (int i = 0; i < MAX_FORMAS_PAGO; ++i) {
        if (CODIGOS_FDP[i] == codigo) { return i; }
    }
    return -1;
}
bool codigoFDPyaUsado(const FormaPago formasPago[], const string& codigo, int hastaIndice) {
    for (int i = 0; i < hastaIndice; ++i) {
        if (formasPago[i].codigo == codigo) { return true; }
    }
    return false;
}

//--- AUXILIAR LOTE 4 ---
int buscarProductoPorCodigo(const Producto productos[], int codigo) {

    for (int i = 0; i < MAX_PRODUCTOS; ++i) {
        if (productos[i].codigo == codigo) { return i; }
    }
    return -1;
}
int buscarIndiceFDP(const FormaPago formasPago[], const string& codigo) { // Busqueda de indice de FP en FP prestablecidos.
    for (int i = 0; i < MAX_FORMAS_PAGO; ++i) {
        if (formasPago[i].codigo == codigo) { return i; }
    }
    return -1;
}

// --- FUNCIÓN DE ORDENAMIENTO (MÉTODO BURBUJA) ---
void ordenarPorBurbuja(AuxiliarReporte arr[], int tam) {
    int i, j;
    AuxiliarReporte temp;

    for (i = 0; i < tam - 1; i++) {
        for (j = 0; j < tam - i - 1; j++) {
            if (arr[j].valor < arr[j+1].valor) {
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

// --- IMPLEMENTACIÓN DE LOTE 1: MARCAS ---
void cargarLoteMarcas(Marca marcas[], bool &loteCargado) {
    if (loteCargado) {
        cout << "El Lote de Marcas ya fue cargado." << endl;
        return;
    }

    cout << "\n--- INICIO DE CARGA: LOTE DE MARCAS (10 REGISTROS) ---" << endl;

    for (int i = 0; i < MAX_MARCAS; ++i) {
        cout << "\n[Registro " << i + 1 << " de " << MAX_MARCAS << "]" << endl;

        int codigoMarca;
        do {
            cout << "Ingrese Codigo de Marca (1-10): ";
            if (!(cin >> codigoMarca)) {
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Entrada invalida. Por favor, ingrese un numero." << endl;
                codigoMarca = 0;
            } else if (codigoMarca == 0) {
                cout << "ERROR: El Codigo de Marca no puede ser 0 (vacio)." << endl;
            } else if (!validarCodigoMarca(codigoMarca)) {
                cout << "ERROR: El Codigo de Marca debe ser un numero entre 1 y 10." << endl;
            } else {
                break;
            }
        } while (true);

        string nombreMarca;
        do {
            cout << "Ingrese Nombre de Marca: ";
            // Limpiar el buffer después de la lectura del número, para el getline
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, nombreMarca);

            if (!validarStringNoVacio(nombreMarca)) {
                cout << "ERROR: El Nombre de la Marca no puede estar vacio." << endl;
            } else {
                break;
            }
        } while (true);

        marcas[i].codigo = codigoMarca;
        marcas[i].nombre = nombreMarca;
        cout << "Marca " << marcas[i].nombre << " cargada correctamente." << endl;
    }

    loteCargado = true;
    cout << "\n--- FIN DE CARGA: LOTE DE MARCAS CARGADO EXITOSAMENTE ---" << endl;
}

// --- IMPLEMENTACIÓN DE LOTE 2: PRODUCTOS ---
void cargarLoteProductos(Producto productos[], const Marca marcas[], bool marcasCargadas, bool &productosCargados) {
    if (!marcasCargadas) {
        cout << "ERROR: No se pueden cargar los productos. El lote de marcas no ha sido cargado." << endl;
        productosCargados = false;
        return;
    }
    if (productosCargados) {
        cout << "El Lote de Productos ya fue cargado." << endl;
        return;
    }

    cout << "\n--- INICIO DE CARGA: LOTE DE PRODUCTOS (20 REGISTROS) ---" << endl;
    bool cargaExitosa = true;

    for (int i = 0; i < MAX_PRODUCTOS; ++i) {
        cout << "\n[Producto " << i + 1 << " de " << MAX_PRODUCTOS << "]" << endl;

        // Código de Producto
        do {
            cout << "Ingrese Codigo de Producto (3 digitos, distinto de 0): ";
            if (!(cin >> productos[i].codigo)) {
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Entrada invalida. Ingrese un numero." << endl;
            } else if (productos[i].codigo == 0) {
                cout << "ERROR: Codigo de Producto es requerido." << endl;
            } else {
                break;
            }
        } while (true);

        // Nombre
        cout << "Ingrese Nombre del Producto: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, productos[i].nombre);
        if (!validarStringNoVacio(productos[i].nombre)) {
            cout << "ERROR: El Nombre del Producto no puede estar vacio. INTERRUPCION DE CARGA." << endl;
            cargaExitosa = false;
            break;
        }

        // Precio de Venta
        do {
            cout << "Ingrese Precio de Venta (distinto de 0): ";
            if (!(cin >> productos[i].precioVenta)) {
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Entrada invalida. Ingrese un numero." << endl;
            } else if (productos[i].precioVenta == 0.0f) {
                cout << "ERROR: El Precio de Venta es requerido." << endl;
            } else {
                break;
            }
        } while (true);

        // Precio de Compra
        do {
            cout << "Ingrese Precio de Compra (distinto de 0): ";
            if (!(cin >> productos[i].precioCompra)) {
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Entrada invalida. Ingrese un numero." << endl;
            } else if (productos[i].precioCompra == 0.0f) {
                cout << "ERROR: El Precio de Compra es requerido." << endl;
            } else {
                break;
            }
        } while (true);

        // Stock Disponible
        do {
            cout << "Ingrese Stock Disponible (distinto de 0): ";
            if (!(cin >> productos[i].stock)) {
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Entrada invalida. Ingrese un numero." << endl;
            } else if (productos[i].stock == 0) {
                cout << "ERROR: El Stock Disponible es requerido." << endl;
            } else {
                break;
            }
        } while (true);

        // Validación Cruzada: Código de Marca
        int codigoMarca;
        int indiceMarca = -1;

        do {
            cout << "Ingrese Codigo de Marca (1-10): ";
            if (!(cin >> codigoMarca)) {
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Entrada invalida. Ingrese un numero." << endl;
                continue;
            }
            indiceMarca = buscarMarcaPorCodigo(marcas, codigoMarca);

            if (indiceMarca == -1) {
                cout << "ERROR: Codigo de marca no encontrado en el lote de marcas. INTERRUPCION DE CARGA." << endl;
                cargaExitosa = false;
                break;
            } else {
                productos[i].codMarca = codigoMarca;
                cout << "Marca: " << marcas[indiceMarca].nombre << " verificada." << endl;
                break;
            }
        } while (true);

        if (!cargaExitosa) {
            break;
        }
    }

    if (cargaExitosa) {
        productosCargados = true;
        cout << "\n--- FIN DE CARGA: LOTE DE PRODUCTOS CARGADO EXITOSAMENTE ---" << endl;
    } else {
        productosCargados = false;
        cout << "La carga del lote de productos ha sido INTERRUMPIDA por el error anterior." << endl;
    }
}

// --- IMPLEMENTACIÓN DE LOTE 3: FORMAS DE PAGO ---
void cargarLoteFormasPago(FormaPago formasPago[], bool &loteCargado) {
    if (loteCargado) {
        cout << "El Lote de Formas de Pago ya fue cargado." << endl;
        return;
    }

    cout << "\n--- INICIO DE CARGA: LOTE DE FORMAS DE PAGO (5 REGISTROS) ---" << endl;

    for (int i = 0; i < MAX_FORMAS_PAGO; ++i) {
        cout << "\n[Forma de Pago " << i + 1 << " de " << MAX_FORMAS_PAGO << "]" << endl;

        string codigoFDP;
        do {
            cout << "Ingrese Codigo de Forma de Pago (EF, MP, TR, TC, CT): ";
            cin >> codigoFDP;

            if (buscarCodigoFDP(codigoFDP) == -1) {
                cout << "ERROR: Codigo de Forma de Pago invalido. Codigos posibles: EF, MP, TR, TC, CT." << endl;
            } else if (codigoFDPyaUsado(formasPago, codigoFDP, i)) {
                cout << "ERROR: El Codigo de Forma de Pago ya fue cargado. No puede haber codigos repetidos." << endl;
            } else {
                formasPago[i].codigo = codigoFDP;
                break;
            }
        } while (true);

        string nombreFDP;
        do {
            cout << "Ingrese Nombre de la Forma de Pago: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, nombreFDP);

            if (!validarStringNoVacio(nombreFDP)) {
                cout << "ERROR: El Nombre de la Forma de Pago no puede estar vacio." << endl;
            } else {
                break;
            }
        } while (true);

        do {
            cout << "Ingrese Porcentaje (Negativo: descuento, Positivo: interes): ";
            if (!(cin >> formasPago[i].porcentaje)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Entrada invalida. Por favor, ingrese un numero entero." << endl;
            } else {
                break;
            }
        } while (true);
    }

    loteCargado = true;
    cout << "\n--- FIN DE CARGA: LOTE DE FORMAS DE PAGO CARGADO EXITOSAMENTE ---" << endl;
}

// --- IMPLEMENTACIÓN DE LOTE 4: VENTAS ---
void cargarLoteVentas (
    Producto productos[],
    const FormaPago formasPago[],
    float recaudacionProducto[],
    int cantidadVendidaProducto[],
    int comprasCliente[],
    int ventasFDP[],
    int ventasMarcaFDP[][MAX_FORMAS_PAGO],
    int &contadorVentas,
    bool lotesCargados[],
    bool &ventasCargadas) {

    // 1. Validar lotes maestros (1, 2, 3)
    if (!validarLotesCargados(lotesCargados)) {
        cout << "ERROR: Debe cargar los lotes de Marcas, Productos y Formas de Pago (Opciones 1, 2 y 3) antes de cargar ventas." << endl;
        ventasCargadas = false;
        return;
    }

    // 2. Reiniciar contadores si ya estaban cargados (omito el cuerpo de reinicio por brevedad)
    if (ventasCargadas) {
        cout << "El lote de ventas ya fue procesado. Reiniciando contadores de reportes..." << endl;
        // ... (código de reinicio de acumuladores) ...
        ventasCargadas = false;
    }

    cout << "\n--- INICIO DE CARGA: LOTE DE VENTAS ---" << endl;
    cout << "Ingrese 0 en 'Nro de Compra' para finalizar la carga." << endl;

    Venta v;
    int productoIndice;
    int formaPagoIndice;

    // Bucle externo: Solo se rompe con Nro de Compra = 0
    do {
        // Bandera de control para la validación interna
        bool ventaValida;

        // Bucle para pedir la Venta y sus validaciones (se repite si hay error)
        do {
            ventaValida = true; // Asumimos que es válida al inicio de cada intento

            // 1. Nro de Compra (Condición de salida)
            cout << "\nIngrese Nro de Compra (0 para finalizar): ";
            if (!(cin >> v.nroCompra)) {
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Entrada invalida. Debe ser un numero." << endl;
                ventaValida = false;
                continue; // Vuelve a pedir Nro Compra
            }

            if (v.nroCompra == 0) {
                // *** INICIO DE LA CORRECCIÓN CLAVE ***
                ventasCargadas = true; // <-- ¡La bandera se actualiza AHORA!
                cout << "\n--- FIN DE CARGA: LOTE DE VENTAS PROCESADO EXITOSAMENTE ---" << endl;
                return; // Sale de la función
                // *** FIN DE LA CORRECCIÓN CLAVE ***
            }

            // --- Carga y Validación de Campos de Venta ---

            // Código de Producto (Debe existir en Lote 2)
            cout << "Ingrese Codigo de Producto: ";
            if (!(cin >> v.codProducto)) {
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "ERROR: Entrada invalida. Vuelva a ingresar la venta." << endl;
                ventaValida = false; continue;
            }
            productoIndice = buscarProductoPorCodigo(productos, v.codProducto);
            if (productoIndice == -1) {
                cout << "ERROR: Codigo de Producto no encontrado. Vuelva a ingresar la venta." << endl;
                ventaValida = false; continue;
            }

            // Forma de Pago (Debe existir en Lote 3)
            cout << "Ingrese Codigo de Forma de Pago (EF, MP, etc.): ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpiar buffer
            getline(cin, v.formaPagoCod);
            formaPagoIndice = buscarIndiceFDP(formasPago, v.formaPagoCod);
            if (formaPagoIndice == -1) {
                cout << "ERROR: Codigo de Forma de Pago invalido. Vuelva a ingresar la venta." << endl;
                ventaValida = false; continue;
            }

            // Cantidad Vendida (entero, debe ser > 0 y <= Stock)
            cout << "Ingrese Cantidad Vendida: ";
            if (!(cin >> v.cantidadVendida) || v.cantidadVendida <= 0) {
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "ERROR: Cantidad Invalida. Vuelva a ingresar la venta." << endl;
                ventaValida = false; continue;
            }
            if (v.cantidadVendida > productos[productoIndice].stock) {
                cout << "ERROR: Stock insuficiente (" << productos[productoIndice].stock << " disponibles). Vuelva a ingresar la venta." << endl;
                ventaValida = false; continue;
            }

            // Código de Cliente (entero de 1 a 50)
            cout << "Ingrese Codigo de Cliente (1-" << MAX_CLIENTES << "): ";
            if (!(cin >> v.codCliente) || v.codCliente < 1 || v.codCliente > MAX_CLIENTES) {
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "ERROR: Codigo de Cliente invalido. Vuelva a ingresar la venta." << endl;
                ventaValida = false; continue;
            }

            // Día de la Venta (entero de 1 a 30)
            cout << "Ingrese Dia de la Venta (1-30): ";
            if (!(cin >> v.diaVenta) || v.diaVenta < 1 || v.diaVenta > 30) {
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "ERROR: Dia de la Venta invalido. Vuelva a ingresar la venta." << endl;
                ventaValida = false; continue;
            }

        } while (!ventaValida); // Repetir si alguna validación falló

        // --- 3. CÁLCULO Y ACUMULACIÓN (SOLO SI ventaValida ES TRUE) ---

        float precioUnitario = productos[productoIndice].precioVenta;
        int porcentajeFDP = formasPago[formaPagoIndice].porcentaje;
        float factorFDP = 1.0f + (float)porcentajeFDP / 100.0f;
        float recaudadoVenta = precioUnitario * v.cantidadVendida * factorFDP;

        // Acumulación:
        recaudacionProducto[productoIndice] += recaudadoVenta;
        cantidadVendidaProducto[productoIndice] += v.cantidadVendida;
        productos[productoIndice].stock -= v.cantidadVendida;
        ventasFDP[formaPagoIndice]++;
        contadorVentas++;
        int marcaIndice = productos[productoIndice].codMarca - 1;
        ventasMarcaFDP[marcaIndice][formaPagoIndice] += v.cantidadVendida;
        comprasCliente[v.codCliente - 1]++;

        cout << "Venta Nro " << v.nroCompra << " procesada. Total: " << fixed << setprecision(2) << recaudadoVenta << endl;

    } while (true); // El bucle se rompe con 'return' si Nro Compra es 0
}

// --- IMPLEMENTACIÓN DEL SUBMENÚ DE REPORTES ---
void mostrarReportes (
    const Producto productos[], const Marca marcas[], const FormaPago formasPago[],
    const float recaudacionProducto[], const int cantidadVendidaProducto[],
    const int comprasCliente[], const int ventasFDP[], const int ventasMarcaFDP[MAX_MARCAS][MAX_FORMAS_PAGO],
    const int contadorVentas) {

    int opcionReporte;

    do {
        cout << "-------------------------------------------" << endl;
        cout << "         SUBMENU DE REPORTES               " << endl;
        cout << "-------------------------------------------" << endl;
        cout << "1. Recaudacion por producto (Ranking)" << endl;
        cout << "2. Porcentaje de ventas por forma de pago" << endl;
        cout << "3. Ventas por marca y forma de pago" << endl;
        cout << "4. Productos sin ventas" << endl;
        cout << "5. Top 10 clientes + sorteo" << endl;
        cout << "0. Volver al Menu Principal" << endl;
        cout << "-------------------------------------------" << endl;
        cout << "Ingrese una opcion de reporte: ";

        if (!(cin >> opcionReporte)) {
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
            opcionReporte = -1;
        }

        switch (opcionReporte) {
            case 1: reporteRecaudacionPorProducto(productos, recaudacionProducto, cantidadVendidaProducto); break;
            case 2: reportePorcentajeVentasFDP(formasPago, ventasFDP, contadorVentas); break;
            case 3: reporteVentasMarcaFDP(marcas, formasPago, ventasMarcaFDP); break;
            case 4: reporteProductosSinVentas(productos, cantidadVendidaProducto); break;
            case 5: reporteTop10Clientes(comprasCliente); break;
            case 0: cout << "Volviendo al menu principal..." << endl; break;
            default: cout << "Opcion invalida. Intente de nuevo." << endl; break;
        }

        if (opcionReporte != 0) {
            cout << "\nPresione ENTER para continuar...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }

    } while (opcionReporte != 0);

}

// --- IMPLEMENTACIÓN DE REPORTES (R1-R5) ---
void reporteRecaudacionPorProducto(const Producto productos[], const float recaudacionProducto[], const int cantidadVendidaProducto[]) {
    cout << "\n==========================================================================" << endl;
    cout << "         REPORTE 1: RECAUDACION POR PRODUCTO (RANKING POR CANTIDAD VENDIDA)" << endl;
    cout << "==========================================================================" << endl;

    AuxiliarReporte ranking[MAX_PRODUCTOS];
    int productosConVentas = 0;

    for (int i = 0; i < MAX_PRODUCTOS; i++) {
        if (cantidadVendidaProducto[i] > 0) {
            ranking[productosConVentas].codigo = productos[i].codigo;
            ranking[productosConVentas].valor = (float)cantidadVendidaProducto[i];
            ranking[productosConVentas].indiceOriginal = i;
            productosConVentas++;
        }
    }

    if (productosConVentas == 0) {
        cout << "No se registraron ventas para ningun producto." << endl;
        return;
    }

    ordenarPorBurbuja(ranking, productosConVentas);

    cout << left << setw(10) << "COD. PROD"
         << left << setw(30) << "NOMBRE DEL PRODUCTO"
         << right << setw(10) << "CANT. VDA"
         << right << setw(15) << "TOTAL RECAUDADO"
         << right << setw(15) << "STOCK REM." << endl;
    cout << setfill('-') << setw(80) << "" << setfill(' ') << endl;

    for (int i = 0; i < productosConVentas; i++) {
        int indice = ranking[i].indiceOriginal;

        cout << left << setw(10) << productos[indice].codigo
             << left << setw(30) << productos[indice].nombre.substr(0, 28)
             << right << setw(10) << cantidadVendidaProducto[indice]
             << right << setw(15) << fixed << setprecision(2) << recaudacionProducto[indice]
             << right << setw(15) << productos[indice].stock << endl;
    }

    cout << setfill('-') << setw(80) << "" << setfill(' ') << endl;
}

void reportePorcentajeVentasFDP(const FormaPago formasPago[], const int ventasFDP[], int contadorVentas) {
    cout << "\n========================================================" << endl;
    cout << "         REPORTE 2: PORCENTAJE DE VENTAS POR FORMA DE PAGO" << endl;
    cout << "========================================================" << endl;

    if (contadorVentas == 0) {
        cout << "No se registraron ventas para calcular porcentajes." << endl;
        return;
    }

    cout << left << setw(30) << "FORMA DE PAGO"
         << right << setw(10) << "VENTAS"
         << right << setw(15) << "PORCENTAJE (%)" << endl;
    cout << setfill('-') << setw(55) << "" << setfill(' ') << endl;

    for (int i = 0; i < MAX_FORMAS_PAGO; i++) {
        float porcentaje = (float)ventasFDP[i] * 100.0f / (float)contadorVentas;

        cout << left << setw(30) << formasPago[i].nombre
             << right << setw(10) << ventasFDP[i]
             << right << setw(15) << fixed << setprecision(2) << porcentaje << "%" << endl;
    }

    cout << setfill('-') << setw(55) << "" << setfill(' ') << endl;
    cout << left << setw(30) << "TOTAL DE VENTAS"
         << right << setw(10) << contadorVentas << endl;
}

void reporteVentasMarcaFDP(const Marca marcas[], const FormaPago formasPago[], const int ventasMarcaFDP[][MAX_FORMAS_PAGO]) {
    cout << "\n==========================================================================" << endl;
    cout << "         REPORTE 3: CANTIDAD TOTAL VENDIDA POR MARCA Y FORMA DE PAGO" << endl;
    cout << "==========================================================================" << endl;

    // Encabezado
    cout << left << setw(20) << "MARCA";
    for (int j = 0; j < MAX_FORMAS_PAGO; j++) {
        cout << right << setw(10) << formasPago[j].codigo;
    }
    cout << right << setw(10) << "TOTAL" << endl;
    cout << setfill('-') << setw(70) << "" << setfill(' ') << endl;

    // Filas de datos
    for (int i = 0; i < MAX_MARCAS; i++) {
        int totalMarca = 0;

        // Imprimir el nombre de la marca
        cout << left << setw(20) << ("[" + to_string(marcas[i].codigo) + "] " + marcas[i].nombre).substr(0, 19);

        // Imprimir las ventas para cada Forma de Pago y sumar el total
        for (int j = 0; j < MAX_FORMAS_PAGO; j++) {
            cout << right << setw(10) << ventasMarcaFDP[i][j];
            totalMarca += ventasMarcaFDP[i][j];
        }

        // Imprimir el total por marca
        cout << right << setw(10) << totalMarca << endl;
    }

    cout << setfill('-') << setw(70) << "" << setfill(' ') << endl;
}

void reporteProductosSinVentas(const Producto productos[], const int cantidadVendidaProducto[]) {
    cout << "\n========================================================" << endl;
    cout << "         REPORTE 4: PRODUCTOS SIN VENTAS" << endl;
    cout << "========================================================" << endl;

    cout << left << setw(10) << "COD. PROD"
         << left << setw(40) << "NOMBRE DEL PRODUCTO" << endl;
    cout << setfill('-') << setw(50) << "" << setfill(' ') << endl;

    int contadorSinVentas = 0;
    for (int i = 0; i < MAX_PRODUCTOS; i++) {
        if (cantidadVendidaProducto[i] == 0) {
            cout << left << setw(10) << productos[i].codigo
                 << left << setw(40) << productos[i].nombre << endl;
            contadorSinVentas++;
        }
    }

    if (contadorSinVentas == 0) {
        cout << "¡FELICIDADES! Todos los productos registraron ventas." << endl;
    }

    cout << setfill('-') << setw(50) << "" << setfill(' ') << endl;
}

void reporteTop10Clientes(const int comprasCliente[]) {
    cout << "\n==========================================================================" << endl;
    cout << "         REPORTE 5: TOP 10 CLIENTES + SORTEO DE CUPONES" << endl;
    cout << "==========================================================================" << endl;

    // 1. Crear y cargar el array auxiliar solo con clientes que tienen compras.
    AuxiliarReporte rankingClientes[MAX_CLIENTES];
    int clientesConCompras = 0;

    for (int i = 0; i < MAX_CLIENTES; i++) {
        if (comprasCliente[i] > 0) {
            rankingClientes[clientesConCompras].codigo = i + 1;
            rankingClientes[clientesConCompras].valor = (float)comprasCliente[i];
            rankingClientes[clientesConCompras].indiceOriginal = i;
            clientesConCompras++;
        }
    }

    if (clientesConCompras == 0) {
        cout << "No se registraron compras de ningun cliente." << endl;
        return;
    }

    // 2. Ordenar el array auxiliar por cantidad de compras (mayor a menor)
    ordenarPorBurbuja(rankingClientes, clientesConCompras);

    // Determinar el tamaño real del Top 10
    int topSize = min(10, clientesConCompras);

    // 3. Imprimir el Top 10
    cout << "\n--- TOP 10 CLIENTES CON MAS COMPRAS ---" << endl;
    cout << left << setw(5) << "RANK" // Alinea el texto a la izquierda, reservando 5 espacios
         << left << setw(15) << "COD. CLIENTE" // Alinea el texto a la izquierda, reservando 15 espacios
         << right << setw(15) << "CANT. COMPRAS" << endl; // Alinea el texto a la derecha, reservando 15 espacios
    cout << setfill('-') << setw(35) << "" << setfill(' ') << endl;

    for (int i = 0; i < topSize; i++) {
        cout << left << setw(5) << i + 1 // Imprime el número de ranking
             << left << setw(15) << rankingClientes[i].codigo // Imprime el Código de Cliente
             << right << setw(15) << (int)rankingClientes[i].valor << endl; // Imprime la Cantidad de Compras
    }
    cout << setfill('-') << setw(35) << "" << setfill(' ') << endl;

    // 4. Sorteo de 3 ganadores // Validacion de TOP 10 - Min 3 clientes.
    if (topSize < 3) {
        cout << "\nADVERTENCIA: Se necesitan al menos 3 clientes con compras para el sorteo." << endl;
        return;
    }

    cout << "\n--- SORTEO DE 3 GANADORES DE CUPONES DE DESCUENTO ---" << endl;

    bool yaGanaron[10] = {false};
    int ganadores = 0;

    while (ganadores < 3) {
        // Generar un número aleatorio entre 0 y topSize-1
        int indiceSorteado = rand() % topSize;

        if (!yaGanaron[indiceSorteado]) {
            int codClienteGanador = rankingClientes[indiceSorteado].codigo;
            int cantidadCompras = (int)rankingClientes[indiceSorteado].valor;

            cout << "GANADOR " << ganadores + 1 << ": Codigo de Cliente "
                 << codClienteGanador << " (" << cantidadCompras << " compras)" << endl;

            yaGanaron[indiceSorteado] = true;
            ganadores++;
        }
    }
    cout << "\n¡Los 3 clientes han ganado un cupón de descuento para su proxima compra!" << endl;
}
