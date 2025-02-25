using ShopApp.Models;

namespace ShopApp.Services
{
    /// <summary>
    /// Provides services for managing products.
    /// </summary>
    public class ProductService : IProductService
    {
        private Dictionary<Guid, Product> _products;

        /// <summary>
        /// Initializes a new instance of the <see cref="ProductService"/> class.
        /// </summary>
        public ProductService()
        {
            _products = new Dictionary<Guid, Product>();
            this.InitProducts();
        }

        private void InitProducts()
        {
            foreach (var item in Enumerable.Range(1, 5))
            {
                var product = new Product()
                {
                    Description = $"Information about {item}",
                    Id = Guid.NewGuid(),
                    Price = Random.Shared.NextDouble()
                };
                _products.Add(product.Id, product);
            }
        }

        /// <summary>
        /// Adds a new product to the collection.
        /// </summary>
        /// <param name="product">The product to add.</param>
        /// <returns>The added product if successful; otherwise, <c>null</c>.</returns>
        public Product? Add(Product product)
        {
            return _products.TryAdd(product.Id, product) ? product : null;
        }

        /// <summary>
        /// Removes a product by its unique identifier.
        /// </summary>
        /// <param name="id">The unique identifier of the product to remove.</param>
        /// <returns>The removed product if found; otherwise, <c>null</c>.</returns>
        public Product? Remove(Guid id)
        {
            if (_products.TryGetValue(id, out Product? product))
            {
                _products.Remove(id);
                return product;
            }
            return null;
        }

        /// <summary>
        /// Edits an existing product.
        /// </summary>
        /// <param name="product">The product with updated information.</param>
        /// <returns>The edited product if found; otherwise, <c>null</c>.</returns>
        public Product? Edit(Product product)
        {
            if (!_products.ContainsKey(product.Id))
            {
                return null;
            }

            _products[product.Id] = product;
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