using System.Text.Json;
using ShopApp.Models;

namespace ShopApp.Services
{
    /// <summary>
    /// Provides services for managing products.
    /// </summary>
    public class ProductService : IProductService
    {
        private Dictionary<Guid, Product> _products;
        private readonly IConfiguration _configuration;
        private readonly string _databaseFilePath;
        private readonly Mutex _fileMutex = new Mutex();

        /// <summary>
        /// Initializes a new instance of the <see cref="ProductService"/> class.
        /// </summary>
        public ProductService(IConfiguration config)
        {
            _configuration = config;
            _databaseFilePath = _configuration["DataBaseFilePath"] ?? "database.txt";
            _products = new Dictionary<Guid, Product>();

            InitFromFile();
        }

        /// <summary>
        /// Reads products from the database file and loads them into memory.
        /// </summary>
        private void InitFromFile()
        {
            if (!File.Exists(_databaseFilePath))
            {
                return;
            }

            try
            {
                _fileMutex.WaitOne();
                string json = File.ReadAllText(_databaseFilePath);
                var products = JsonSerializer.Deserialize<List<Product>>(json);

                if (products != null)
                {
                    foreach (var product in products)
                    {
                        _products[product.Id] = product;
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error reading from file: {ex.Message}");
            }
            finally
            {
                _fileMutex.ReleaseMutex();
            }
        }

        /// <summary>
        /// Saves the current product list to the database file.
        /// </summary>
        private void WriteToFile()
        {
            try
            {
                _fileMutex.WaitOne();

                string json = JsonSerializer.Serialize(_products, new JsonSerializerOptions { WriteIndented = true });
                File.WriteAllText(_databaseFilePath, json);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error save file: {ex.Message}");
            }
            finally
            {
                _fileMutex.ReleaseMutex();
            }
        }

        /// <summary>
        /// Adds a new product to the collection.
        /// </summary>
        /// <param name="product">The product to add.</param>
        /// <returns>Adds a new product to the collection and saves the update to disk if successful; otherwise, <c>null</c>.</returns>
        public Product? Add(Product product)
        {
            if (_products.TryAdd(product.Id, product))
            {
                WriteToFile();
                return product;
            }
            return null;
        }

        /// <summary>
        /// Removes a product by its unique identifier.
        /// </summary>
        /// <param name="id">The unique identifier of the product to remove.</param>
        /// <returns>The removed product and updates the database if found; otherwise, <c>null</c>.</returns>
        public Product? Remove(Guid id)
        {
            if (_products.TryGetValue(id, out Product? product))
            {
                _products.Remove(id);
                WriteToFile();
                return product;
            }
            return null;
        }

        /// <summary>
        /// Edits an existing product.
        /// </summary>
        /// <param name="product">The product with updated information.</param>
        /// <returns>The edited product and updates the database if found; otherwise, <c>null</c>.</returns>
        public Product? Edit(Product product)
        {
            if (!_products.ContainsKey(product.Id))
            {
                return null;
            }

            _products[product.Id] = product;
            WriteToFile();
            return product;
        }

        /// <summary>
        /// Searches for a product by its unique identifier.
        /// </summary>
        /// <param name="id">The unique identifier of the product to search for.</param>
        /// <returns>The found product if it exists; otherwise, <c>null</c>.</returns>
        public Product? Search(Guid id)
        {
            _products.TryGetValue(id, out var product);
            return product;
        }
    }
}