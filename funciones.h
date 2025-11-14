#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <iostream>
#include <iomanip>
#include <limits> // Para numeric_limits

using namespace std;

// --- DEFINICIÓN DE STRUCTS ---

struct Marca
{
    int codigo;
    string nombre;
};
struct Producto
{
    int codigo;
    string nombre;
    float precioVenta;
    float precioCompra;
    int stock;
    int codMarca;
};
struct FormaPago
{
    string codigo;
    string nombre;
    int porcentaje;
};
struct Venta
{
    int nroCompra;
    int codProducto;
    string formaPagoCod;
    int cantidadVendida;
    int codCliente;
    int diaVenta;
};
struct AuxiliarReporte
{
    int codigo;
    float valor;
    int indiceOriginal;
};

// --- DEFINICIÓN DE CONSTANTES PARA TAMAÑO DE ARRAYS ---

const int MAX_MARCAS = 10;
const int MAX_PRODUCTOS = 20;
const int MAX_FORMAS_PAGO = 5;
const int MAX_CLIENTES = 50;
const int MAX_VENTAS = 500;
const string CODIGOS_FDP[MAX_FORMAS_PAGO] = {"EF", "MP", "TR", "TC", "CT"};

// --- PROTOTIPOS DE FUNCIONES ---

// Funciones de Control y Menú
void ejecutarPrograma(); // Función controladora principal.
void mostrarMenuPrincipal(); // Opciones Menu.

// Funciones de Lotes
void cargarLoteMarcas(Marca marcas[], bool &loteCargado);
void cargarLoteProductos(Producto productos[], const Marca marcas[], bool marcasCargadas, bool &productosCargados);
void cargarLoteFormasPago(FormaPago formasPago[], bool &loteCargado);
void cargarLoteVentas(
    Producto productos[],
    const FormaPago formasPago[],
    float recaudacionProducto[], int cantidadVendidaProducto[], int comprasCliente[],
    int ventasFDP[], int ventasMarcaFDP[][MAX_FORMAS_PAGO], int &contadorVentas,
    bool lotesCargados[], bool &ventasCargadas);

// Funciones Auxiliares (Búsqueda, Validación y Ordenamiento)
bool validarCodigoMarca(int codigo);
bool validarStringNoVacio(const string& texto);
int buscarMarcaPorCodigo(const Marca marcas[], int codigo);
int buscarProductoPorCodigo(const Producto productos[], int codigo);
int buscarIndiceFDP(const FormaPago formasPago[], const string& codigo);
bool validarLotesCargados(const bool lotesCargados[]);
void ordenarPorBurbuja(AuxiliarReporte arr[], int tam);
int buscarCodigoFDP(const string& codigo);
bool codigoFDPyaUsado(const FormaPago formasPago[], const string& codigo, int hastaIndice);

// Funciones de Reportes
void mostrarReportes(
    const Producto productos[], const Marca marcas[], const FormaPago formasPago[],
    const float recaudacionProducto[], const int cantidadVendidaProducto[],
    const int comprasCliente[], const int ventasFDP[], const int ventasMarcaFDP[][MAX_FORMAS_PAGO],
    const int contadorVentas);
void reporteRecaudacionPorProducto(const Producto productos[], const float recaudacionProducto[], const int cantidadVendidaProducto[]);
void reportePorcentajeVentasFDP(const FormaPago formasPago[], const int ventasFDP[], int contadorVentas);
void reporteVentasMarcaFDP(const Marca marcas[], const FormaPago formasPago[], const int ventasMarcaFDP[MAX_MARCAS][MAX_FORMAS_PAGO]);
void reporteProductosSinVentas(const Producto productos[], const int cantidadVendidaProducto[]);
void reporteTop10Clientes(const int comprasCliente[]);

#endif // FUNCIONES_H_INCLUDED
